#ifndef ASYNCR_SERIAL_H
#define ASYNCR_SERIAL_H

#include <Arduino.h>
#include "Logger.h"
#define INPUT_BUFF_SIZE 203

class AsyncrSerial {
private:
	String inputBuff="";
	HardwareSerial* ptSerPort;
	bool isValidData;
	Logger *myLogger;
public:
	AsyncrSerial(HardwareSerial* serPort,Logger* myLogger);
	char * getMessage();
	bool GetIsValidData();


};
#endif
