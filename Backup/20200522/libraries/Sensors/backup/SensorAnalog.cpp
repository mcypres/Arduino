/*
 * SensorAnalog.cpp
 *
 *  Created on: 5 oct. 2015
 *      Author: marc
 */

#include "SensorAnalog.h"

SensorAnalog::SensorAnalog(int pinNum,float refVoltage) {
	mPinNum=pinNum;
	mCoefConv=refVoltage/1023;
}
void SensorAnalog::getData(){
	Value=analogRead(mPinNum)*mCoefConv;
}
float SensorAnalog::getData(){
	return analogRead(mPinNum)*mCoefConv;
}
String SensorAnalog::toString(){
	return String(Value,2);
}
String SensorAnalog::toNmea(){
	return "";
}

SensorAnalog::~SensorAnalog() {
	// TODO Auto-generated destructor stub
}

