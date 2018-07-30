#ifndef EFT_SETUPNEOPIXEL_H
#define EFT_SETUPNEOPIXEL_H

#include <Arduino.h>
#include <string>
#include "setup/ISetupNotif.h"
#include <NeoPixelBus.h>

namespace eft {

#define WS281X_FEATURE NeoGrbFeature
#define WS281X_METHOD NeoEsp8266AsyncUart800KbpsMethod

/**
 * Class used during setup using a RGB Neopixel led strip
 */
class SetupNeoPixel : public ISetupNotif {

public:
SetupNeoPixel(int pin, int countLed);
~SetupNeoPixel();

/**
 * We cannot connect to the network
 */
void error();

/**
 * Give the ip of the ESP
 */
void client(String ip);

/**
 * We have successfully connected to the network
 */
void success();

/**
 * We are in progress of connecting
 */
void inprogress();


private:
NeoPixelBus<WS281X_FEATURE, WS281X_METHOD> *neo;
int countLed;

};

}

#endif
