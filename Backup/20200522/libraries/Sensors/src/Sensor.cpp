/*
 * Sensor.cpp
 *
 *  Created on: 1 août 2015
 *      Author: marc
 */

#include "Sensor.h"
//namespace sensors {

Sensor::Sensor(HardwareSerial *ptOutputSer) {
	this->ptOutputSer=ptOutputSer;
}
Sensor::Sensor(HardwareSerial *ptOutputSer,Logger *myLogger){
	this->ptOutputSer=ptOutputSer;
	this->myLogger=myLogger;
}
Sensor::Sensor(Logger *myLogger){	
	this->myLogger=myLogger;
}
void Sensor::checksum(unsigned char* ptr,char* ptChk,int msgLength){
     unsigned char checksum;
     checksum = 0;
     if ( *ptr == '$' ) ptr++;
     for (int i=0;i<msgLength;i++){
 
		_LOG_PRINT(V,"i =",i);
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

}
String Sensor::getSensorDetail(){
	return sensorName;
}
Sensor::~Sensor() {
	ErrFlag=false;
	ErrorMsg="";
}
//} /* namespace sensors */
