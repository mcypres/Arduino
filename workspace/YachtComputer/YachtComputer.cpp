// Do not remove the include below
#include "YachtComputer.h"
#include "SensorBmp183.h"
SensorBmp183* myBaro;

//The setup function is called once at startup of the sketch
void setup()
{
	  Serial3.begin(9600);    // Bluetooth
	  Serial2.begin(4800);    // NMEA
	  //Serial1.begin(4800);    // Seatalk
	  Serial.begin(9600);     // USB

	 myBaro= new SensorBmp183();}

// The loop function is called in an endless loop
void loop()
{
	String msg;
	myBaro->getData();
    msg=myBaro->toNmea();
     Serial3.println(msg);
     Serial.println(msg);
     Serial.println();}
