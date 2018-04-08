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


