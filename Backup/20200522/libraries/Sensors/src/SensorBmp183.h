/*
 * SensorBmp183.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SENSORBMP183_H_
#define SENSORBMP183_H_
#include "Arduino.h"
#include "Sensor.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <stdio.h>


class SensorBmp183: public Sensor {
public:
	SensorBmp183(HardwareSerial *ptOutputSer,Logger *myLogger);
	String toString();
	char* toNmea();
	void getData();
	//String getSensorDetail();

	virtual ~SensorBmp183();
private:
	Adafruit_BMP085 bmp;
	float pression,temperature;
	unsigned long lastTime;

};

#endif /* SENSORBMP183_H_ */
