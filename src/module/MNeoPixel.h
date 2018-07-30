//
// This class is used to get current time.
//
#ifndef EFT_NEOPIXEL_H
#define EFT_NEOPIXEL_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "comm/IComm.h"
#include "comm/ServerUtil.h"
#include "setup/SetupNeoPixel.h"
#include <NeoPixelBus.h>

namespace eft {

class RgbColorExt {
public:
RgbColorExt(RgbColor colorStart);
RgbColorExt(RgbColor colorStart, RgbColor colorEnd);
~RgbColorExt();
RgbColor colorStart;
RgbColor colorEnd;

};

/**
 *
 */
class MNeoPixel : public IComm {

public:
/**
 *
 */
MNeoPixel(int pin, int countLed);
~MNeoPixel();
/**
 *
 */
void setup(String nameAP, String nameFX, Config *config);
/**
 *
 */
String commType();
/**
 * Describe this module
 */
void commFx(JsonObject *o, JsonArray *ja);
/**
 * Redister webserver and mqtt calls
 */
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
/**
 * Main loop
 */
void loop();
/**
 * Serve http calls
 */
void servHTTP();
/**
 * Received from the mqtt channel we have subscribed to
 */
void mqttServ(String mess);
/**
 * Set a color using a RgbColor objects
 */
bool col(int ledId, RgbColor color);
/**
 * Set the rgb color of one of the led. Please note that the change is effective when we call the loop of this class.
 */
bool col(int ledId, int red, int green, int blue);
/**
 * Use the string format (same as the one sent through mqtt and web api)
 */
bool col(String mess, JsonObject* ja = NULL);

private:
ESP8266WebServer *webServer;
MQTT *mqtt;
String nameFX;
bool changeColorSub(String mess, JsonObject* ja = NULL);

int pin;
NeoPixelBus<WS281X_FEATURE, WS281X_METHOD> *neo;
int countLed;
bool doShow = false;
RgbColorExt getColor(String color);
RgbColor getSingleColor(String color);
HtmlColor htmlColor;

};

}

#endif
