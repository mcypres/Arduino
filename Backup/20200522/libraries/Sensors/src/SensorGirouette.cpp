/*SensorGirouette.cpp
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */


#include <SensorGirouette.h>


SensorGirouette::SensorGirouette(Logger* myLogger,SensorMagneto* sensor1,SensorMagneto* sensor2=NULL):Sensor(myLogger){
	if (sensor1==NULL){
		_LOG_PRINT(M, "sensor1 :", "NULL");
	}
	this->sensor1=sensor1;
	
	this->sensor2=sensor2;
	if( sensor2==NULL){
		dual=false;
	}
	else{
		dual=true;
	}
	
}
void SensorGirouette::init(uint8_t range){
	if (dual==true){
		Tca::tcaselect(0);
		sensor1->init(range);
		Tca::tcaselect(1);
		sensor2->init(range);
	}
	else{
		sensor1->init(range);	
	}
}
void SensorGirouette::getData(){
	if (dual==true){
		Tca::tcaselect(0);
		_LOG_PRINT(M, "tcaselect :", "0");
	}
	_LOG_PRINT(M, "getData() :", "sensor1->getData()");
	sensor1->getData();
	sensors_event_t event1=sensor1->getEvent();
	_LOG_PRINT(M, "event1 x :", event1.magnetic.x);
	_LOG_PRINT(M, "event1 y :", event1.magnetic.y);
	if (dual==true){ 
		Tca::tcaselect(1);
		_LOG_PRINT(M, "getData() :", "sensor2->getData()");
		sensor2->getData();
		sensors_event_t event2=sensor2->getEvent();
		_LOG_PRINT(M, "event2 x :", event2.magnetic.x);
		_LOG_PRINT(M, "event2 y :", event2.magnetic.y);
		Heading=SensorMagneto::getHeadingDegrees( event1,0,event2.magnetic.x, event2.magnetic.y);
	}
	else{
		Heading=SensorMagneto::getHeadingDegrees( event1,0,0,0);//provisoire;
	}
	
}

String SensorGirouette::toString(){
	return "todo";
}
float SensorGirouette::getHeading(){
	getData();
	return Heading;
}



