/*
 * SensorHMagneto.h
 *
 *  Created on: 10/12/2019
 *      Author: marc
 */

#ifndef SensorHMagneto_H_
#define SensorHMagneto_H_
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Sensor.h"
#include <stdio.h>
#define MAX_VALUE 10000
#define MIN_VALUE -10000


class SensorMagneto: public Sensor {
public:
	SensorMagneto(String sensorName,Logger* myLogger);
	SensorMagneto(String sensorName,Logger* myLogger,int declinationAngleDegres,float declinationAnglemin,float xOffset,float yOffset,float zOffset,int id);
	
	String toString(sensors_event_t event);
	
	void ComputeData(sensors_event_t event,float declinationAngle );
	virtual void getData()=0;
	virtual void init(uint8_t range )=0;
	sensors_event_t getEvent(){
		getData();
		return event;
	};
	float getHeadingDegrees(){
		return headingDegrees;
	};
	static float getHeadingDegrees(sensors_event_t event,float declinationAngle,float xOffset, float yOffset);	

	String getSensorDetail(sensors_event_t event);
	virtual ~SensorMagneto();
protected:
	sensors_event_t event;
	float magnetudexy,magnetude;
	float declinationAngle ;
	float headingDegrees;
	float heading;
	float minx,miny,minz,maxx,maxy,maxz;
	float xOffset, yOffset,zOffset;


};

#endif /* SensorHMC5883 */
