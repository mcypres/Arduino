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
#include <SensorMagneto.h>
/*#include <avr/interrupt.h>

extern unsigned long lastTime;
extern volatile unsigned long GanemoCount;
extern void anemoInterupt();
*/

class SensorHMC5883: public SensorMagneto {
public:
	SensorHMC5883(String sensorName,
					Logger* myLogger,
					int declinationAngleDegres,
					float declinationAnglemin,
					float xOffset,
					float yOffset,
					float zOffset,
					int id);
	//String toNmea();
//	float getSpeed();
	void getData();
	String toString();
//	String getSensorDetail();
	virtual ~SensorHMC5883();
private:
	Adafruit_HMC5883_Unified mag;



};

#endif /* SensorHMC5883 */
