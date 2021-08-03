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
#include <avr/interrupt.h>
#define ANALOG_INPUT
#ifdef ANALOG_INPUT
#include "SensorAnalog.h"
#endif
extern unsigned long lastTime;
extern volatile unsigned long GanemoCount;
extern void anemoInterupt();
class SensorHMC5883: public Sensor {
public:
	SensorHMC5883(float declinationAngleDegres,float declinationAnglemin,float xOffset,float yOffset,float 			zOffset,int id);
	String toString();
	String toNmea();
	float getHeadingDegrees();
	float getSpeed();
	void getData();
	String getSensorDetail();
	virtual ~SensorHMC5883();
private:
	Adafruit_HMC5883_Unified mag;
	sensors_event_t event;
	float magnetudexy,magnetude;
	float declinationAngle ;
	float headingDegrees;
	float heading;
	float speed;
	unsigned long lastTime;
	float minx,miny,minz,maxx,maxy,maxz;
	float xOffset, yOffset,zOffset;
#ifdef ANALOG_INPUT
	SensorAnalog* cap;
	SensorAnalog* vitesse;
#endif

};

#endif /* SensorHMC5883 */
