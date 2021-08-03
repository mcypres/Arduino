/*
 * SPinout.h
 *
 *  Created on: 17/08/2017
 *      Author: marc
 */

#ifndef PINGROUP_H_
#define PINGROUP_H_
#include "Arduino.h"
#define GROUP_SIZE 4
#define NMEA_TYPE_SIZE 4
enum PINTYPE{ ANLOG,LOGICAL};
enum  NMEA_TYPE{INT,VOL};

}


public  class pinGroup{
	//char nmeaTypeName[][]={"INT","VOL";
	int pinNumber[GROUP_SIZE];
	int groupNumber;
	PINTYPE pinType;
	boolean input;
	boolean mPullup;
	char pinName[11];
		int pinNumber[GROUP_SIZE];
		char unit;
		char NMEAtype[NMEA_TYPE_SIZE ];
		float scale[GROUP_SIZE];
		pinDesc mDescritor;
	pinGroup(pinDesc descritor){}
	String toString(){};
	char geUnit() return unit;
	int getGroupNuber() return groupNumber;
	
}
#endif /* PINGROUP_H_ */