/*
 * Sensor.h
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#ifndef UT61E_H_
#define UT61E_H_
#define LOG_SERIAL Serial
#include "Arduino.h"
//#include <SoftwareSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_IN_LENGTH  14
#define START_POS_VALUE 1
#define END_POS_VALUE 6
#define UNIT_POS 6
#define ECHELLE_POS 0
#define DECIMAL_CHAR "."
#define INFO_POS 7
#define  SGN_BIT 4

class ut61e {
public:
	ut61e(HardwareSerial *inStream);
	int getData();
	void decode();
	char getEchelle(){
		return echelle;
	};
	char getUnit(){
		return unit;
	};
	float getValue(){
		return value;
	};
	String getSelection();
protected:
	HardwareSerial *mInStream;
	String msgIn,lastMsgIn;
	char echelle;
	char unit;
	float value;
	String selection[16]={String("A"),String("Diode"),String("Hz, %"),String("Ohm"),String("°C"),String("Pieps"),String("F"),String(""),String(""),String("A"),String(""),String("V, mV"),String(""),String("µA"),String("ADP"),String("mA")};
	int base[16]=		 {2			 ,-1		     ,3				 ,3			   ,-1			,-1				,2			,-1		   ,-1		  ,2		  ,-1		 ,1				 ,-1		,3			 ,-1		   ,2};
	
	/*Bereich (Byte[0]):V, mV		Ohm		F		Hz		µA		mA		A		%
	'0'					2.2000		220.00	22.000n	220.00	220.00µ	22.000m	10.000	100.0*
	'1'					22.000		2.2000k	220.00n	2200.0	2200.0µ	220.00m	-		100.0*
	'2'					220.00		22.000k	2.2000µ	-	-	-		-		-
	'3'					1000.0		220.00k	22.000µ	22.000k	-		-		-		100.0*
	'4'					220.00m		2.2.00M	220.00µ	220.00k	-		-		-		100.0*
	'5'					-			22.000M	2.2000m	2.2000M	-		-		-		100.0*
	'6'					-			220.00M	22.000m	22.000M	-		-		-		100.0*
	'7'					-			-		220.00m	220.00M	-		-		-		100.0*
base 					1			2			2		3		3	2		2		3*/
	//virtual String toString();
	//virtual String toNmea()=0;
	static void checksum(unsigned char* ptr,char* ptChk,int msgLength);
	
	String ErrorMsg;
	boolean ErrFlag;
public:
	//String getName();
	//void  getdecode();
	//virtual String getUt61eDetail();
	//String getErrorMsg(){return ErrorMsg;}
	//boolean getError(){return ErrFlag;}
	// ~ut61e();
};

#endif /* UT61E_H_ */
