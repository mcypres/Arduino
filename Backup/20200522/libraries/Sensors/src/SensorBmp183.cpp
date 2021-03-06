/*
 * SensorBmp183.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include "SensorBmp183.h"
#include <Adafruit_BMP085.h>


#define NMEA_MPT_HEADER "PAMPR,"
#define PRESSURE_UNIT "hPa,"
#define TEMP_UNIT ",°C"
#define NMEA_MPT_MSG "$PAMPR,00000.00,hPa,000.00, C*     "
//********************0123456789012345678901234567890
#define PRESSION_OFFSET 8
#define TEMP_OFFSET 20
#define MSG_LENGTH_T_CHECKSUM 28
#define CHECKSUM_OFFSET 30
#define BARO_TIME_INTERVAL 1000

SensorBmp183::SensorBmp183(HardwareSerial *ptOutputSer,Logger *myLogger):Sensor(ptOutputSer,myLogger) {
	pression=0;
	temperature=0;
	lastTime=0;
	sensorName="Bmp183";
	//myLogger->printLog(D,(char*) " __FILE__ ","__LINE__",(char*)" constructor","***\n",sensorName);
	//_LOG_PRINT(D," constructor",sensorName);
	ErrFlag=false;
	if (!bmp.begin()) {
		ErrorMsg="Could not find a valid BMP085 sensor, check wiring!";
		ErrFlag=true;
	  }
}
String SensorBmp183::toString(){
	return sensorName;
}
/*String getSensorDetail()
{
	return sensorName;
}*/
char* SensorBmp183::toNmea(){

	
	  static char msg[]=NMEA_MPT_MSG;
	   _LOG_PRINT(V," msg",msg);
	  char tmp[10];
	  _LOG_PRINT(V," msg",msg);
	  dtostrf(pression,7, 2,tmp);
	  strncpy(msg+PRESSION_OFFSET,tmp,7);
	  _LOG_PRINT(V," msg",msg);
	  dtostrf((float) temperature,5,2,tmp);
	  strncpy(msg+TEMP_OFFSET,tmp,5);
	  _LOG_PRINT(V," msg",msg);
	  checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
	  //ptLogSer->println(msg);
	  _LOG_PRINT(D," msg",msg);
	  ptOutputSer->println(msg);
	  return msg;
}
void SensorBmp183::getData(){
	//ptLogSer->print("getData start ");
	pression=bmp.readPressure()/100;
	_LOG_PRINT(V," pression",pression);
	temperature= bmp.readTemperature();
	_LOG_PRINT(V," temperature",temperature);
}

SensorBmp183::~SensorBmp183() {
	// TODO Auto-generated destructor stub
}

