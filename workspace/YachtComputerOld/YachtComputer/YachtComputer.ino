#include <Wire.h>

//The setup function is called once at startup of the sketch
#include "SensorBmp183.h"
#include <Adafruit_BMP085.h>
#include <stdio.h>
#include <String.h>

SensorBmp183* myBaro;
void setup()
{
	  Serial3.begin(9600);    // Bluetooth
	  Serial2.begin(4800);    // NMEA
	  //Serial1.begin(4800);    // Seatalk
	  Serial.begin(9600);     // USB
char tmp[3];
           itoa((int)18,tmp,HEX);
	 myBaro= new SensorBmp183();
}

// The loop function is called in an endless loop
void loop()
{
	String msg;
	myBaro->getData();
    msg=myBaro->toNmea();
     Serial3.println(msg);
     Serial.println(msg);
     Serial.println();

}
