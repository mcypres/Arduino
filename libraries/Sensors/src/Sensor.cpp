/*
 * Sensor.cpp
 *
 *  Created on: 1 août 2015
 *      Author: marc
 */

#include "Sensor.h"
//namespace sensors {

Sensor::Sensor(String sensorName,HardwareSerial *ptOutputSer,MqttInterface* interface) {
	this->ptOutputSer=ptOutputSer;
	this->interface=interface;
	this->sensorName=sensorName;
}

Sensor::Sensor(String sensorName,HardwareSerial *ptOutputSer,Logger *myLogger,MqttInterface* interface){
	this->ptOutputSer=ptOutputSer;
	this->myLogger=myLogger;
	this->interface=interface;
	this->sensorName=sensorName;
}

Sensor::Sensor(String sensorName,Logger *myLogger,MqttInterface* interface){	
	this->myLogger=myLogger;
	this->interface=interface;
	this->sensorName=sensorName;
}

char* Sensor::toNmea(){
	 
	return "not implemented for this device";
}

void Sensor::publish(){
	if(interface==NULL){
		return false;	 		
	}
	return interface->publish("Error","publish not implemented for this device"); 			 	
}


void Sensor::checksum(unsigned char* ptr,char* ptChk,int msgLength,int* ptChkInt=NULL){
     unsigned char checksum;
     checksum = 0;
     _LOG_PRINT(V,"ptr input =",(char *)ptr);
     _LOG_PRINT(V,"msgLength input =",msgLength);
     if ( *ptr == '$' ) ptr++;
     for (int i=0;i<msgLength;i++){
 
		_LOG_PRINT(V,"i =",i);
		if(i==0||i==1|| i==msgLength-1)
		_LOG_PRINT(V,"  *ptr :",(char)*ptr);
		_LOG_PRINT(V,"  checksum :",checksum);
		
       checksum ^= *ptr;
       //Serial.println(checksum,HEX);
       
       _LOG_PRINT_HEX(V,"  checksum :",checksum);
       ptr++;
     }
     char tmp[3];
     itoa((int)checksum,tmp,HEX);
     _LOG_PRINT(V,"  tmp :",tmp);
     strncpy((char*)ptChk,tmp,3);
     if (ptChkInt!=NULL){
     	*ptChkInt=(int)checksum;
     }
     
}

String Sensor::getSensorDetail(){
	return sensorName;
}

Sensor::~Sensor() {
	ErrFlag=false;
	ErrorMsg="";
}
//} /* namespace sensors */
