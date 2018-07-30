# ESP-EFTNeoPixel

ESP-EFTNeoPixel is a wrapper around the NeoPixel library.

This library is an add-on to ESP-EFTCore.
https://github.com/gabrielklein/ESP-EFTCore

And using the excellent work from https://github.com/Makuna/NeoPixelBus.


# Example

```c

// If you want to show the setup using neopixel band
#include "setup/SetupNeoPixel.h"

// If you want to use a neopixel band
#include "module/MNeoPixel.h"

// Build the hub using
...
eft::Hub hub("MyNeopixelExample");

// In your setup function
void setup() {

  ...

  // Configure the NeoPixel
  // First parameter is the pin where the NeoPixel is connected
  // Second parameter is the number of leds in you NeoPixel band.
  eft::SetupNeoPixel setupNeoPixel(D4, 16);
  eft::ISetupNotif *isn = &setupNeoPixel;
  hub.setup(isn);

  // You don't need this code once hub.setup is finished
  // (It's why we construct setupNeoPixel without a new).

  // Then add the NeoPixel band to the hub
  hub.add("neo", new eft::MNeoPixel(D4, 16));

}

// In your loop function don't forget
void loop() {
        hub.loop();
}
```

For more information, see full documentation [here](doc/MNeoPixel.md).
