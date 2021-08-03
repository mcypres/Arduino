/*
 * SensorHMC5883Duo.cpp
 *
 *  Created on: 04 aout. 2019
 *      Author: marc
 */

#include "SensorHMC5883Duo.h"
//#include "SensorHMC5883.h"
#include "tca.h"

SensorHMC5883Duo::SensorHMC5883Duo(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					float declinationAngleDegres,
					float declinationAnglemin,
					int id,
					float xOffset,
					float yOffset,
					float zOffset,
					float speedCoef
					) :Sensor(ptOutputSer,myLogger){
	
	_LOG_PRINT(V, &"SensorHMC5883Duo", "Start constructor");				
	this->xOffset=xOffset;
	this->yOffset=yOffset;
	this->zOffset=zOffset;
	ErrFlag=false;
	Tca::tcaselect(0);
	delay(10);						
	mag0 =new SensorHMC5883(ptOutputSer,
			myLogger,
			declinationAngleDegres,
			declinationAnglemin,
			id);
	if (mag0->getError()){
		ErrFlag=true;
		ErrorMsg="Could not find first valid HMC5883 sensor, check wiring!";
	}
	delay(10);		
	Tca::tcaselect(1);
	delay(10);
	mag1 =new SensorHMC5883(ptOutputSer,
			myLogger,
			declinationAngleDegres,
			declinationAnglemin,
			id+1);
	if (mag1->getError()){
		ErrFlag=true;
		ErrorMsg=ErrorMsg+"Could not find second valid HMC5883 sensor, check wiring!";
	}	
	_LOG_PRINT(V, &"SensorHMC5883Duo", "End constructor");						
}

String SensorHMC5883Duo::toString(){
	delay(10);
	Tca::tcaselect(0);
	delay(10);
	String ret=" \n Sensor0 :\n\t"+mag0->toString();
	delay(10);
	Tca::tcaselect(1);
	delay(10);
	return ret +"\n Sensor1 :\n\t"+mag1->toString();
}

char* SensorHMC5883Duo::toNmea(){
	return "";
}

String SensorHMC5883Duo::getSensor2Detail(){
	return mag0->getSensorDetail();
}

sensors_event_t  SensorHMC5883Duo::getEvent2(){
	return mag1->getEvent();
}

 void SensorHMC5883Duo::getData(){
	_LOG_PRINT(V,"getData getEvent :","Start");
	if (mag0==NULL){
		_LOG_PRINT(M,"mag0==null :","");
		return;
	}
	delay(10);
	Tca::tcaselect(0);	
	delay(10);
	_LOG_PRINT(V,"getData getEvent :","event0 Start");
	event0=mag0->getEvent();
	_LOG_PRINT(V,"getData getEvent :","envent0 end event1 Start");	
	if (mag1==NULL){
		_LOG_PRINT(M,"mag1==null :","");
		return;
	}
	delay(10);	
	Tca::tcaselect(1);
	delay(10);
	event1=mag1->getEvent();	
	_LOG_PRINT(V,"getData getEvent :"," event1 end");
  	headingDegrees =SensorHMC5883::getHeadingDegrees( event0,0,event1.magnetic.x+xOffset,event1.magnetic.y+yOffset);
}

float SensorHMC5883Duo::getHeadingDegrees(){
	getData();
	return headingDegrees;
}
char* SensorHMC5883Duo::eepromParamtoString(){
	return "";
}

SensorHMC5883Duo::~SensorHMC5883Duo() {
	delete mag0;
}
