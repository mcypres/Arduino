/*
 * SensorBmp183.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include "SensorBmp183.h"
#include <Adafruit_BMP085.h>
SensorBmp183::SensorBmp183() {
	pression=0;
	temperature=0;
	lastTime=0;
	if (!bmp.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	  }

}
String SensorBmp183::toString(){
	return "";
}
String SensorBmp183::toNmea(){

	  char msg[]=NMEA_MPT_MSG;
	  char tmp[10];
	  dtostrf(pression,7, 2,tmp);
	  strncpy(msg+PRESSION_OFFSET,tmp,7);
	  dtostrf((float) temperature,5,2,tmp);
	  strncpy(msg+TEMP_OFFSET,tmp,5);
	  checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
	  return msg;
}
void SensorBmp183::getData(){
	if (lastTime +BARO_TIME_INTERVAL < millis()){
	        lastTime =millis();
	        pression=bmp.readPressure()/100;
	        temperature= bmp.readTemperature();
	}
}

SensorBmp183::~SensorBmp183() {
	// TODO Auto-generated destructor stub
}

