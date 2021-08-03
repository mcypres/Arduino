/*
 * Sensor.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef SENSOR_H_
#define SENSOR_H_
#include "Arduino.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<MqttInterface.h>
//#include"/home/marc/Arduino/libraries/Logger/Logger.h"
#include"Logger.h"

#define MSG_Max_LENGHTS      85
#define NMEA_SEPARATOR ","
#define NMEA_HEADER "$"
#define AIS_HEADER "!"
#define MSG_TAIL "*"
#define SEATALK_HAEDER "#"
#define MQTT_TOPIC_MAX_SIZE 200
#define MQTT_PAYLOAD_MAX_SIZE 200


class Sensor {
public:
	Sensor(String sensorName,HardwareSerial *ptOutputSer,Logger *myLogger,MqttInterface* interface=NULL);
	Sensor(String sensorName,Logger *myLogger,MqttInterface* interface=NULL);
	Sensor(String sensorName,HardwareSerial* ptOutputSer,MqttInterface* interface=NULL);
	
	virtual String toString()=0;
	virtual char* toNmea();
	virtual void getData()=0;
	void  publish();
	String eepromParamtoString() {return  String(sensorName+ "as no Eeprom param");}
	void checksum(unsigned char* ptr,char* ptChk,int msgLength,int* ptChkInt=NULL);
	
	static int freeRam () {
		extern int __heap_start, *__brkval; 
		int v; 
		return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
	}
	 ~Sensor();
protected:
	String sensorName;
	String ErrorMsg="";
	boolean ErrFlag=false;
	HardwareSerial *ptOutputSer;
	Logger *myLogger;
	MqttInterface* interface;
public:
	
	String getName();
	virtual String getSensorDetail();
	String getErrorMsg(){return ErrorMsg;}
	boolean getError(){return ErrFlag;}
};

#endif /* SENSOR_H_ */
