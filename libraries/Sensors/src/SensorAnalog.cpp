/*
 * SensorAnalog.cpp
 *
 *  Created on: 5 oct. 2015
 *      Author: marc
 */

#include "SensorAnalog.h"

SensorAnalog::SensorAnalog(String sensorName,HardwareSerial *ptOutputSer,int pinNum,float refVoltage):Sensor(sensorName,ptOutputSer) {
	//super(ptOutputSer);
	mPinNum=pinNum;
	pinMode(pinNum, INPUT);
	mCoefConv=refVoltage/1023;
}
void SensorAnalog::getData(){
	Value=analogRead(mPinNum)*mCoefConv;
}
float SensorAnalog::getValue(){
	return analogRead(mPinNum)*mCoefConv;
}
String SensorAnalog::toString(){
	return String(Value,2);
}
char* SensorAnalog::toNmea(){
	
}

SensorAnalog::~SensorAnalog() {
	// TODO Auto-generated destructor stub
}

