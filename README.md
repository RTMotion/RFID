# RFID
This small script will show you how to use the RC522 module on an Arduino UNO or Nano.

### Description

Here you will find the sketch and the used library.
The library goes to the standard Arduino-libraries directory.
For more info about the lib, please visit: https://github.com/miguelbalboa/rfid

The basic idea behind the sketch is:
- Recognize a RFID-card
- Check, if the card is valid
- If so, turn on a LED
- Otherwise flash the LED rapid

###Hardware

**Required components:**
- Arduino UNO or Nano
- RC522 RFID module
- LED
- 220 Ohm resistor
- Jumper wires

**Connection for the RC522:**

Arduino Pin | RC522 Pin
------------|--------------
9			| RST
10			| SDA
11			| MOSI
12			| MISO
13			| SCK
3V3			| 3V3
GND			| GND



For further instructions, please visit my blog: