
/*Nmea.cpp
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */

#include <Nmea.h>
#include"Logger.h"

Nmea::Nmea(Logger *myLogger){
	this->myLogger=myLogger;
}

void Nmea::checksum(unsigned char* ptr,char* ptChk,int msgLength){
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
	

 
