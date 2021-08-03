#ifndef NMEA_H
#define NMEA_H
#define NMEA_MSG_LENGTH  80

#include"Logger.h" 
 class Nmea{
public:
 	Nmea(Logger *myLogger,char* msgFrame,int nbParam,float* paramList,signed char paramSize[],unsigned char paramPrec[],int paramPos[]);
 	char* ToNmea();
 	void checksum(unsigned char* ptr,char* ptChk,int msgLength);
private:
	Logger *myLogger;
 	int nbParam;
 	char* msg;
 	float* paramList;
 	signed char* paramSize;
 	unsigned char* paramPrec;
 	int* paramPos;
	char* firstParamPos;
	char* checksumPos;
};
#endif