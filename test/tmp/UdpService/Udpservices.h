/*
 * Udpservices.h
 *
 *  Created on: 31 mai 2016
 *      Author: marc
 */

#ifndef UDPSERVICES_H_
#define UDPSERVICES_H_
#include "Arduino.h"
#include <stdio.h>
#include"HardwareSerial.h"
#include "CircBuf.h"


#define SSID        "wifitelsur_MWAC"
 #define PASSWORD    "0019C70D4077X"
 #define HOST_NAME   "192.168.4.2"
 #define HOST_PORT   5416
 #define REM_PORT  5416
 //#define LOG_BAUDRATE 115200
 //#define ESP_BAUDRATE 115200
 #define DELAY 10000
 #define TIMEOUT 1000
 #define CIRC_BUFF_LENGH 256
 #define SEND_DELAY 1000
 
class Udpservices {
private:
	HardwareSerial * mEspSerial;
	HardwareSerial* mLogSerial;
	bool mErr=false;
	CircBuf  mInputBuff;

public: 
	//Udpservices(HardwareSerial* EspSeria,HardwareSerial* LogSerial,long espBaudratel=ESP_BAUDRATE,long logBaudRate=LOG_BAUDRATE);
	Udpservices(HardwareSerial* EspSeria,HardwareSerial* LogSerial);
	bool init(String ssid=SSID,
			String passwor=PASSWORD,
			String hostName=HOST_NAME,
			long hostPort=HOST_PORT,
			long remPort=REM_PORT
			);
	String readLine();
	void println(String str);
	bool emptyBuffer();
	virtual ~Udpservices();
private:
	void LogReponce();
	
};



#endif /* UDPSERVICES_H_ */
