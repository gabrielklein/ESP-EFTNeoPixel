# MNeoPixel

This project is a wrapper around NeoPixelBus library to use it in the ESP-EFT framework.
It's a way to connect a WS2813 or WS2812 (RGB strip).

This module is using NeoPixelBus - In theory RGB and RGBW leds like SK6812, WS2811, WS2812 and WS2813 that are commonly referred to as NeoPixels and two wire protocol RGB like APA102 commonly referred to as DotStars are compatible.

For more information, please check https://github.com/Makuna/NeoPixelBus.

# Where to buy and how to install

https://www.aliexpress.com/item/LED-WS2813-1m-4m-5m-New-product-WS2813-Smart-led-pixel-strip-IP30-IP67-DC5V-Zwart/32685607810.html

or

https://www.aliexpress.com/item/10-1000pcs-4-Pin-WS2812B-WS2812-LED-Chip-Heatsink-5V-5050-RGB-WS2811-IC-Built-in/32634454437.html


# Integration

A few leds could be connected directly to signal port D4 and powered with 3.3v.

I haven't tested if it really works on other pins than D4.

The red, geen, blue level of each of them can be controlled individually.

I recommend to use a SN74AHCT125N if you want to use more than 5 at full power!

http://circuits.datasheetdir.com/102/SN74AHCT125-pinout.jpg

1: 1OE: Connect to GND

2: 1A: Input with port D4

3: 1Y: Output to "signal" pin of the leds.

7: GND to GND

14: Vcc to +5V

Connect the leds to the 5v power supply!



You can include the library using

```

#include "module/MNeoPixel.h"

[...]

hub.add("neo", new eft::MNeoPixel(D4, 16));

```

## C++ integration

You can change the color of the leds using

eft::MNeoPixel neo(D4, 16)

1)

neo.col(String mess)

Where

"mess" is using the same format as mqtt or data format.

Example: neo.col("0 red, 1-5 00ff00, 6 0000ff");

Please note that the change of color is effective only when the "loop" of the hub is called.

2)

You can set the color of a led by using the following call.

neo.col(int ledId, RgbColor color);

With
* RgbColor c = RgbColor(red,green,blue);

Example
* RgbColor c = RgbColor(red,green,blue);
* neo.col(3, c);


Please note that the change of color is effective only when the "loop" of the hub is called.

3)

You can set the color of a led by using the following call.

neo.col(int ledId, int red, int green, int blue);

Example: neo.col(2, 255, 0, 0);

Set the third led to red (255,0,0).

Please note that the change of color is effective only when the "loop" of the hub is called.

# Format of the data String

You need to send some data using the following format

{leds to be modified} {color}

Color is either defined using the "Red-Green-Blue" hex format or giving the color in full text.

Led could be one of the following

* * : Set all leds
* n : Set one of the led (n). 0 is the first led.
* n-m : Set a range of leds (n is the first led, m the last one).
* x : Set all leds (separated by spaces).

Color is one of the following

* FF0000 : Set the color to red.
* 00FF00 : Set the color to green.
* 0000FF : Set the color to blue.
* Red : Other way to set the color to red.
* F00 : You can use a 3 hex format.

You can create an evolution of color using -

Some examples
* Red-Blue : Create colors from red to blue
* FF0000-0000FF : Create colors from red to blue


Full examples

* Example: 1 00FF00
Set the second led to green.

* Example: 2-5 00FF00
Set the 3rd, 4th, 5th, 6th leds to green.

* Example: 1-10 FF0000-0000FF
Set leds 2 to 11 to a color from red to blue.

* Example: x FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF FF0000 00FF00 0000FF
Set color 1 to red, 2 to green, 3 to blue, ...

# Core Web API

## /api/{name}?d={datastring}

You can change the color using a call like.

Set a all leds to red: /api/neo?d=* red

Set a all leds to colors between red to green: /api/neo?d=* red-blue
or /api/neo?d=* FF0000-0000FF

Set the first three colors to red, green blue

/api/neo?d=0 red,1 green,2 blue
or /api/neo?d=x red green blue
or /api/neo?d=x FF0000 00FF00 0000FF


The response of a call is the colors that were changed.

http://{ip}/api/neo?d=x%20f00-00f

```

{
  "0": "#11000",
  "1": "#0f000",
  "2": "#0e000",
  "3": "#0d000",
  "4": "#0c000",
  "5": "#0b000",
  "6": "#0a000",
  "7": "#09000",
  "8": "#07000",
  "9": "#06000",
  "10": "#05000",
  "11": "#04000",
  "12": "#03000",
  "13": "#02000",
  "14": "#01000",
  "15": "#00001",
  "q": "x 100-001"
}

```

# MQTT

You can send a mqtt message to this channel.

/{baseTopic}/{nameFX} (Example: /mymodule/neo)

The message sent should follow the following format: {datastring}

Example sending the following message: 0-15 FF0000-00FF00

Will create a gradient of colors between leds 0 and 15 from red to blue.
