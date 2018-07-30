#include "MNeoPixel.h"


namespace eft {

RgbColorExt::~RgbColorExt() {
        //
};


RgbColorExt::RgbColorExt(RgbColor colorStart)
{
        this->colorStart = colorStart;
        this->colorEnd= colorStart;
};

RgbColorExt::RgbColorExt(RgbColor colorStart, RgbColor colorEnd) {
        this->colorStart = colorStart;
        this->colorEnd= colorEnd;
};


MNeoPixel::MNeoPixel(int pin, int countLed) {
        this->countLed = countLed;
        this->pin = pin;
};

MNeoPixel::~MNeoPixel() {
        if (this->neo != NULL) {
                delete(this->neo);
        }

};

void MNeoPixel::commFx(JsonObject *o, JsonArray *ja) {

        (*o)["description"] = "Control a RGB strip (NeoPixel)";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "Configure a RGB multicolor strip";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "d", "string", "Colors of the strip (Ex: x red green blue ff9900)");

        }


};


void MNeoPixel::setup(String nameAP, String nameFX, Config *config) {

        this->nameFX = nameFX;
        this->neo = new NeoPixelBus<WS281X_FEATURE, WS281X_METHOD>(countLed, pin);

        // Create strange issues
        //pinMode(BUILTIN_LED1, OUTPUT);

        this->neo->Begin();
        this->neo->ClearTo(RgbColor(0, 0, 0));
        this->neo->Show();
};

String MNeoPixel::commType() {
        return "MNeoPixel";
};

void MNeoPixel::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {


        this->webServer = webServer;
        this->mqtt = mqtt;

        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servHTTP();
                });

        mqtt->subscribe(nameFX, "", [&] (String mess) {
                        this->mqttServ(mess);
                });


};

void MNeoPixel::servHTTP() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        String d = webServer->arg("d");
        if (d.length() >0) {
                col(d, &root);
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};

void MNeoPixel::mqttServ(String mess) {

        mess.trim();
        mess.toLowerCase();

        col(mess);

};

bool MNeoPixel::col(String mess, JsonObject* ja) {

        int start = 0;
        int countSuccess = 0;

        for (int i=0; i<mess.length(); i++) {
                if (mess.charAt(i) == ',') {
                        if (changeColorSub(mess.substring(start, i), ja))
                                countSuccess++;
                        start = i+1;
                }
        }
        changeColorSub(mess.substring(start), ja);
        return countSuccess > 0;
};

bool MNeoPixel::changeColorSub(String mess, JsonObject* ja) {

        mess.trim();
        if (mess.length() ==0) {
                Serial.println("No message found");
                return false;
        }
        char cc[8];

        // Special format for
        // x FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF
        if (mess.charAt(0) == 'x') {

                if (mess.indexOf('-')>=0) {
                        // A "range", something like "x col1-col2"
                        // this message is processed like * col1-col2.
                }
                else {
                        int start = 1;
                        int colId = 0;

                        for (int i=1; i<mess.length(); i++) {
                                if (mess.charAt(i) == ' ' || i == mess.length()-1) {
                                        String rgb = mess.substring(start, i);
                                        if ( i == mess.length()-1) {
                                                rgb = mess.substring(start);
                                        }
                                        if(rgb.length()>0) {
                                                RgbColorExt c = getColor(rgb);
                                                this->neo->SetPixelColor(colId, c.colorStart);

                                                //Serial.println("C: "+String(colId)+" "+rgb+" *** "+String(c.R, HEX)+" "+String(c.G, HEX)+" "+String(c.B, HEX));

                                                if (ja != NULL) {
                                                        HtmlColor hc(c.colorStart);
                                                        hc.ToString<HtmlShortColorNames>(cc, 8);
                                                        (*ja)[String(colId)] = String(cc);
                                                }

                                                colId++;
                                        }
                                        start = i+1;
                                }

                        }

                        RgbColor c = RgbColor(0,0,0);
                        HtmlColor hc(c);
                        hc.ToString<HtmlShortColorNames>(cc, 8);
                        for (int kk=colId; kk<countLed; kk++) {
                                this->neo->SetPixelColor(kk, c);
                                if (ja != NULL) {
                                        (*ja)[String(kk)] = String(cc);
                                }
                        }
                        doShow = true;
                        // this->neo->Show();


                        //if (ja != NULL) {
                        //        (*ja)["q"] = mess;
                        //}
                        return true;
                }
        }


        if (ja != NULL) {
                (*ja)["q"] = mess;
        }

        int p = mess.indexOf(' ');
        if (p<0) {
                Serial.println("Space not found in message");
                return false;
        }

        String m1 = mess.substring(0, p);
        String m2 = mess.substring(p+1);

        int min = 0;
        int max = countLed-1;

        RgbColorExt c = getColor(m2);

        p = m1.indexOf('-');

        // Probably a message like "x red-blue"
        if (m1.equals("x")) {
                //Nothing to be done
        }
        //        *00FF 00
        else if (m1.equals("*")) {
                //Nothing to be done
        }
        // 1 00FF 00
        else if (p<0) {
                min = max = m1.toInt();
        }
        // 2-5 00FF 00
        else {
                String n1 = m1.substring(0, p);
                String n2 = m1.substring(p+1);
                min = n1.toInt();
                max = n2.toInt();
        }

        if (min<0)
                min=0;
        if (max>countLed-1)
                max = countLed-1;
        if (max<min)
                max = min;

        float step = (max>min) ? 1.0/(float)(max-min) : 1.0;
        float progress = 0;
        for (int i=min; i<=max; i++) {
                RgbColor c2 = RgbColor::LinearBlend(c.colorStart, c.colorEnd, progress);
                //Serial.println(":::"+String(progress)+" -- "+String(c2.R)+" -- "+String(c2.G)+" -- "+String(c2.B));
                this->neo->SetPixelColor(i, c2);
                if (ja != NULL) {
                        HtmlColor hc(c2);
                        hc.ToString<HtmlShortColorNames>(cc, 8);
                        (*ja)[String(i)] = String(cc);
                }
                progress+=step;
        }
        doShow = true;


        //HtmlColor hc(c);
        //hc.ToString<HtmlShortColorNames>(cc, 8);
        //Serial.print(m1);
        //Serial.print(" ");
        //Serial.print(m2);
        //Serial.print(" ");
        //Serial.print(min);
        //Serial.print(" to ");
        //Serial.print(max);
        //Serial.print(": ");
        //Serial.println(String(cc));

}

bool MNeoPixel::col(int ledId, RgbColor color) {
        this->neo->SetPixelColor(ledId, color);
        return true;
}

bool MNeoPixel::col(int ledId, int red, int green, int blue) {
        RgbColor c = RgbColor(red,green,blue);
        this->neo->SetPixelColor(ledId, c);
        return true;
}

RgbColorExt MNeoPixel::getColor(String rgb) {
        int p = rgb.indexOf('-');
        if (p<0) {
                return RgbColorExt(getSingleColor(rgb));
        }

        String c1 = rgb.substring(0, p);
        String c2 = rgb.substring(p+1);

        return RgbColorExt(getSingleColor(c1),getSingleColor(c2));
}

RgbColor MNeoPixel::getSingleColor(String rgb) {

        bool addD = false;
        if (rgb.length() == 3||rgb.length() == 6) {
                addD = true;
                for (int i=0; i<rgb.length(); i++) {
                        char c = rgb.charAt(i);
                        if ((c>='0' && c<='9')||(c>='a' && c<='f')||(c>='A' && c<='F')) {
                                // Ok it's hex
                        }
                        else {
                                addD = false;
                        }
                }
                if (addD)
                        rgb = "#"+rgb;
        }

        htmlColor.Parse<HtmlColorNames>(rgb);
        RgbColor c = RgbColor(htmlColor);
        return c;
}


void MNeoPixel::loop() {

        // Do it in the main loop (in interupt sometime create some problems)
        if (doShow) {
                doShow = false;
                this->neo->Show();
        }

};


}
