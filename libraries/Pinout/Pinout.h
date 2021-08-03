/*
 * SPinout.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef PINOUT_H_
#define PINOUT_H_
#include "Arduino.h"

enum PINTYPE{ ANLOG,LOGICAL};
struct pinDesc {
	int pinNumber;
	int groupNumber;
	PINTYPE pinType;
	boolean input;
	boolean pullup;
	char pinName[11];
}


public static class pinout{
	
}
#endif /* PINOUT_H_ */