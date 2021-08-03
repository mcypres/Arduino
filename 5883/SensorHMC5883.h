/*
 * SensorHMC5883.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SensorHMC5883_H_
#define SensorHMC5883_H_
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "Sensor.h"
#include <stdio.h>
#include"Logger.h" 

//#include <avr/interrupt.h>
//#define ANALOG_INPUT
/*#ifdef ANALOG_INPUT
#include "SensorAnalog.h"
#endif*/
//extern unsigned long lastTime;
extern volatile unsigned long GanemoCount;
extern void anemoInterupt();

class SensorHMC5883: public Sensor {
public:
	
	float xOffset, yOffset,zOffset,speedCoef;
	
	SensorHMC5883(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					float declinationAngleDegres,
					float declinationAnglemin,
					int id,
					float xOffset=0,
					float yOffset=0,
					float zOffset=0,
					float speedCoef=1);
	String toString();
	// 
	String toString(sensors_event_t *event);
	char* eepromParamtoString();
	char* toNmea();
	char* toNmea(float HeadingDegrees,float Speed);
	static float getHeadingDegrees(sensors_event_t event,
					float declinationAngle=0,
					float xOffset=0,
					float yOffset=0);
	float getSpeed();
	void getData();
	sensors_event_t   getEvent();
	String getSensorDetail();
	//String getErrorMsg(){return ErrorMsg;}
	//boolean getError(){return ErrFlag;}
	virtual ~SensorHMC5883();
protected:
	Adafruit_HMC5883_Unified mag;
	sensors_event_t event;	
	float magnetudexy,magnetude;
	float declinationAngle ;
	float headingDegrees;
	float heading;
	float speed;
	unsigned long mLastTime;
	float minx,miny,minz,maxx,maxy,maxz;
	
	void init(Adafruit_HMC5883_Unified);

#ifdef ANALOG_INPUT
	SensorAnalog* cap;
	SensorAnalog* vitesse;
#endif

};

#endif /* SensorHMC5883 */
