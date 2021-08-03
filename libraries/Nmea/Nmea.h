
 /*Nmea.h
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */


#ifndef NMEA_H
#define NMEA_H

//#include"Logger.h"
 #include"/home/marc/Arduino/libraries/Logger/Logger.h"
#include <stdio.h>

class Nmea{

public:
	Nmea(Logger* myLogger);
	//virtual String toNmea()=0;
	void checksum(unsigned char* ptr,char* ptChk,int msgLength);	
protected:
	Logger *myLogger;												
};



#endif
