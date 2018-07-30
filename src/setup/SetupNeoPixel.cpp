#include "SetupNeoPixel.h"


namespace eft {

SetupNeoPixel::SetupNeoPixel(int pin, int countLed) {

        this->countLed = countLed;
        this->neo = new NeoPixelBus<WS281X_FEATURE, WS281X_METHOD>(countLed, pin);

        // Create strange issues
        //pinMode(BUILTIN_LED1, OUTPUT);

        this->neo->Begin();
        //this->neo->ClearTo(RgbColor(0, 0, 0));
        this->neo->Show();

        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(10,0,0));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(0,10,0));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        for (int i=0; i<countLed; i++) {
                this->neo->SetPixelColor(i, RgbColor(0,0,10));
                this->neo->Show();
                delay(5);
                this->neo->SetPixelColor(i, RgbColor(0,0,0));
        }
        this->neo->Show();

        RgbColor c1 = RgbColor(10,20,30);
        this->neo->ClearTo(c1);

};

SetupNeoPixel::~SetupNeoPixel() {
        if (neo ==  NULL)
                return;
        RgbColor c1 = RgbColor(0,0,0);
        this->neo->ClearTo(c1);
        this->neo->Show();
        delete(this->neo);
};

void SetupNeoPixel::error() {
        RgbColor c1 = RgbColor(10,0,0);
        this->neo->ClearTo(c1);
        this->neo->Show();
};

void SetupNeoPixel::success() {
        RgbColor c1 = RgbColor(0,10,0);
        this->neo->ClearTo(c1);
        this->neo->Show();
};

void SetupNeoPixel::client(String ip) {
        if (neo ==  NULL)
                return;
        RgbColor c1 = RgbColor(0,0,0);
        this->neo->ClearTo(c1);
        this->neo->Show();
        delete(this->neo);
        neo = NULL;
};

void SetupNeoPixel::inprogress() {
        RgbColor c1 = RgbColor(0,0,10);
        this->neo->ClearTo(c1);
        this->neo->Show();

}


}
