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

#define NMEA_MPT_HEADER "PAMPR,"
#define PRESSURE_UNIT "hPa,"
#define TEMP_UNIT ",°C"
#define NMEA_MPT_MSG "$PAMPR,00000.00,hPa,000.00,°C*     "
//********************0123456789012345678901234567890
#define PRESSION_OFFSET 8
#define TEMP_OFFSET 20
#define MSG_LENGTH_T_CHECKSUM 28
#define CHECKSUM_OFFSET 31
#define BARO_TIME_INTERVAL 1000

class SensorBmp183: public Sensor {
public:
	SensorBmp183();
	String toString();
	String toNmea();
	void getData();
	//String getSensorDetail();

	virtual ~SensorBmp183();
private:
	Adafruit_BMP085 bmp;
	float pression,temperature;
	unsigned long lastTime;

};

#endif /* SENSORBMP183_H_ */
