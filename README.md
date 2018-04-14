# Arduino Nano Bluetooth Chat
Serial chat between arduino nano and mobile phone using CC2541BLE bluetooth board. 

![Communication Phone->Serial console](communication.jpg)


## Hardware
---
I bought a Bluetooth 4.0 BLE module on [*www.aliexpress.com*](https://www.aliexpress.com/item/Android-IOS-BLE-Bluetooth-4-0-HM-10-CC2540-CC2541-6Pin-Serial-Wireless-Module-DC-5V/32837215626.html) and realized that it was not an original HM-10 module but a clone called BLE CC41-A bluetooth module based on the TI CC2541 chip.
But that did not really matter to me at that time because I had no clue what the difference was anyway.
These clones have a sort of slightly different/limited AT command set. 
You can find the documentation of these devices here:
* [Datasheet of the original HM-10 module](http://duinopeak.com/wiki/images/c/cd/Bluetooth4_en.pdf)
* [CC41-A bluetooth module specification](https://img.banggood.com/file/products/20150104013145BLE-CC41-A%20Spefication.pdf)
* [AT-Command subset of the CC41-A bluetooth module](http://img.banggood.com/file/products/20150104013200BLE-CC41-A_AT%20Command.pdf).
* [Datasheet of the TI CC2541 ble bluetooth chip](http://www.ti.com/lit/ds/symlink/cc2541.pdf)<br>
I have also added these documents to this github repository.

![CC2541 Frontside](CC2541_front.jpg) ![CC2541 Backside](CC2541_back.jpg)

### Wiring
---
|      Nano   |  Bluetooth |
|-------------|------------|
| PIN 10 (RX) |    TXD     |
| PIN 11 (TX) |    RXD     |
| PIN 27 (+5V)|    VCC     |
| PIN 29 (GND)|    GND     |


![Wiring Arduino Nano -> Bluetooth module](wiring.jpg)


The *STATE* pin of the bluetooth module is connected to a LED, which indicates clearly when a mobile phone is connected.
The *EN* pin could be used to disconnect devices by pulling it to low.

**One word about voltage levels of the bluetooth module:**<br>
Even when you can supply the module with 3.6 to 6 volt the voltage *output level* of the module is *3.3* volt.
Some people on the internet hooked the TRX pin up to 5 volts and it seemed not to harm the module. 
I made a voltage divider out of a small 2K potentiometer that reduced the 5 volt of the arduino TX pin 11 to 3.3 volts.
The bigger problem was that the arduino nano did not always recognize the 3.3 volt input of the bluetooth module on it's RX pin 10.
When I measured the voltages I realized that the voltages were not 5 volt from the nano but 4.6 volt and the 3.3 volt of the bluetooth module were in fact 3.1 volt.
Probably that was not enough to be recognized as high on the nano RX pin 10 and so the communication sometimes only worked in one direction.
So I hooked up an external stabilized power supply on pin 27 of the nano and VCC of the bluetooth module and everything worked as expected.
I think it would probably the best to power the arduino and the bluetooth module both with stablized 3.3 volt from an external power supply.
Also keep in mind that the output of the bluetooth module is 3.3 volt when you calculate the resistor for the status LED.

### Arduino Nano
I bought a cheap chinese clone of the arduino nano on [*aliexpress*](https://www.aliexpress.com/item/1PCS-MINI-USB-Nano-V3-0-ATmega328P-CH340G-5V-16M-Micro-controller-board-for-arduino-NANO/32848298184.html) and encased it in a 3D printed housing from [*thingiverse*](https://www.thingiverse.com/thing:959057).


## Software
---

### Arduino
---
You can download the newest arduino id from [*here*](https://www.arduino.cc/en/Main/Software) and follow the installations instructions [*here*](https://www.arduino.cc/en/Guide/HomePage).

### Serial Bluetooth Terminal
---
I used [*this serial bluetooth terminal*](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) from [*Kai Morich*](http://www.kai-morich.de/android/). 
It was the only bluetooth terminal I found that was able to handle the BLE 4.0 paring without PIN.
It seems as if there is still mismatch between what the standard says and how it is implemented by different chip producers and how it is handled by software like android for example.
You can read more about that and how to troubleshoot it [*here*](http://www.kai-morich.de/android/ble_troubleshooting.html).
***It is important that you use a bluetooth software that is able to talk with the BLE modules, otherwise you might end up debugging on the wrong place! :)***<br>

## MISC
---
* As far as I know only one client can connect to the bluetooth module at a time. When the module is connected to a client already it does not seem to be scanable anymore.
* I also realized that AT commands can sometimes? be issue from the serial console to the bluetooth chip as long as no phone is connected to it.
When a phone is connected to it the commands were routed to the phone instead.
* The chip/device offers 4 different security modes where TYPE0 is default. TYPE0 means no pairing at all. Many bluetooth application have not implemented that standard, yet. See also [*here*](http://www.kai-morich.de/android/ble_troubleshooting.html).

## Code
---
```C++
//////////////////////////////////////////////////
//
// Programm lets you chat over the serial
// console with an bluetooth device.
// Used to test the HM-10/ CC2540 /CC2541
// BLE bluetooth devices.
// Make sure that you use an app that actually
// is able to make the pairing/bonding with
// the BLE bluetooth devices, otherwise
// it can lead to frustration ;).
// This one worked for me:
// https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal
//
///////////////////////////////////////////////////
#include <SoftwareSerial.h>

#define softRX 10
#define softTX 11

// we read in from the bluetooth device on the
// software serial pins (softRX+softTX) and
// give it out on the build in hardware serial port
// and vice versa
SoftwareSerial btSerial(softRX, softTX);
String msg;

///////////////////////////////////////////////////
//
///////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("Hello seriell monitor!");
  btSerial.begin(9600);
  btSerial.println("Hello seriell bluetooth device!");
  //  btSerial.println("AT+HELP");
  //  btSerial.println("AT+TYPE0");  // no pin/pairing/bonding needed
  //  btSerial.println("AT+RESET");  // would AT commands work here ?
}

///////////////////////////////////////////////////
//
///////////////////////////////////////////////////
void loop()
{
  while (btSerial.available() > 0)
  {
    Serial.println( btSerial.readString() );
  }

  while (Serial.available() > 0)
  {
    msg = Serial.readString();
    Serial.println( "-> "+msg );  // local echo
    btSerial.print( msg );
  }
}
```

## License
---
Distributed under the Apache License Version 2.0.

## References
---
* https://www.heise.de/developer/artikel/Bluetooth-Auf-den-blauen-Zahn-gefuehlt-3361763.html
* https://create.arduino.cc/projecthub/achindra/bluetooth-le-using-cc-41a-hm-10-clone-d8708e
* http://nerdclub-uk.blogspot.co.at/2016/02/working-with-cheap-bluetooth-btle4.html
* http://img.banggood.com/file/products/20150104013200BLE-CC41-A_AT%20Command.pdf
* http://www.instructables.com/id/How-to-Use-Bluetooth-40-HM10/
* http://www.martyncurrey.com/bluetooth-modules/
* http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/
* https://www.elv.de/Bluething-Board-Daten%C3%BCbertragung-per-Bluetooth-zwischen-Arduino-Nano-und-Smartphone,-PC-oder-einem-anderen-Arduino-Nano/x.aspx/cid_726/detail_61762
* http://www.kai-morich.de/android/ble_troubleshooting.html
* https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal
* http://www.circuitstoday.com/arduino-nano-tutorial-pinout-schematics
* https://www.thingiverse.com/thing:959057

## Authors:
---
* quirxi (https://github.com/quirxi)
