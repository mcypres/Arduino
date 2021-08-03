/*
 * TestNmea.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef TestNmea_H_
#define TestNmea_H_
#include "Arduino.h"
#include "Sensor.h"

class TestNmea: public Sensor{
public:	TestNmea(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					  char* msgModel,
					 const int portList[],
					 const int posList[],
					 const float coefList[],
					 int listSyze);
	
	String toString();
	char* toNmea();
	void getData();
	static void  printstring(char * str);

private:

	char* msgModel;
	const int* portList;
	const int* posList;	
	const float* coefList;
	int listSyze;
	int msgCksLenght;
	char * CksPos;
};

#endif //TestNmea_H_