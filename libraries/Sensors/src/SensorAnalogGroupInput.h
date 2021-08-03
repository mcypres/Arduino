 
/*
 * SensorAnalogGroupInput.h
 *
 *  Created on: 15/08/2017
 *      Author: marc
 */

#ifndef SENSORANALOGGROUPINPUT_H_
#define  SENSORANALOGGROUPINPUT_H_
#include "Arduino.h"
#include "Sensor.h"

 #define NBENTRY 4 
#define NMEA_INT_MSG "$PAINT, ,      ,A, ,      ,A, ,      ,A, ,      ,A*     "
//********************012345678901234567890123456789012345678901234567890123456789
#define NMEA_VOL_MSG "$PAVOL, ,      ,V, ,      ,V, ,      ,V, ,      ,V*     "
//********************012345678901234567890123456789012345678901234567890123456789
#define FIRST_BLOCK_OFFSET 7 
#define SIGNE_OFFSET 0
#define VALUE_OFFSET 2
#define VALUE_SAVE_CHAR 8
#define BLOCK_LENGTH 11
#define MSG_LENGTH_T_CHECKSUM 49
#define CHECKSUM_OFFSET 51
#define ZERO_OFFSET 512
//#define FULL_SCALE 1024
#define NB_ADC_STEPS 1023
enum  NMEA_TYPE{INT,VOL} ;
 class SensorAnalogGroupInput: public Sensor {
 
 public:
 	SensorAnalogGroupInput(String sensorName,HardwareSerial *ptOutputSer,
 							Logger *myLogger,
 							const char unit,
 							const NMEA_TYPE type,
 							const int *pinNumber,
 							const float *fullScale,
 							const int *offset,
 							const float refVoltage,
 							const int refSource,
 							MqttInterface* interface=NULL);
 	String toString();                   
	char* toNmea();
	void publish();
	void getData();
	float getEntryValue(int entry);
	int getLastRead(int entry){
		return mLastRead[entry];
	};
	
private:
 	const char NmeaTypeList[2][4]={"INT","VOL"};
 	float mFullScale[NBENTRY];
 	int mOffset[NBENTRY];
 	float mRefVoltage;
 	int mRefSource;
 	int mPinNumber[NBENTRY] ;
 	int mLastRead[NBENTRY];
 	NMEA_TYPE mType;
 	void buildNmeaMsg(char * msg);
   //
  //analogReference(INTERNAL1V1);
  //analogReference(INTERNAL);
  //analogReference(INTERNAL2V56);
  //analogReference(EXTERNAL);
 };
#endif //SENSORANALOGGROUPINPUT_H_
