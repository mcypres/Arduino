/*
 * Sensor.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSG_Max_LENGHTS      85
#define NMEA_SEPARATOR ","
#define NMEA_HEADER "$"
#define AIS_HEADER "!"
#define MSG_TAIL "*"
#define SEATALK_HAEDER "#"

class Sensor {
protected:
	Sensor();
	virtual String toString()=0;
	virtual String toNmea()=0;
	virtual void getData()=0;
	static void checksum(unsigned char* ptr,char* ptChk,int msgLength);
	 ~Sensor();
	String Name;
public:
	String getName();

};

#endif /* SENSOR_H_ */
