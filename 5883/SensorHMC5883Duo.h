/*
 * SensorHMC5883Duo.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SensorHMC5883Duo_H_
#define SensorHMC5883Duo_H_

#include "Sensor.h"
#include "SensorHMC5883.h"
#include "tca.h"

class SensorHMC5883Duo: public Sensor{
public:
	SensorHMC5883Duo(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					float declinationAngleDegres,
					float declinationAnglemin,
					int id,
					float xOffset=0,
					float yOffset=0,
					float zOffset=0,
					float speedCoef=1
					);
	float getHeadingDegrees();
	char* eepromParamtoString();
	String getSensor2Detail();
	sensors_event_t  getEvent2();
	String toString();
	char* toNmea();
	void getData();

	~SensorHMC5883Duo();
protected:
	SensorHMC5883 * mag0, *mag1;	
	sensors_event_t  event0,event1;
	float magnetudexy,magnetude;
	float declinationAngle ;
	float headingDegrees;
	float heading;
	float speed;
	float xOffset, yOffset,zOffset,speedCoef;

};

#endif
