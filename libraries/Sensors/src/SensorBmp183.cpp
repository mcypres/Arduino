/*
 * SensorBmp183.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */
#define NO_LOG
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

SensorBmp183::SensorBmp183(String sensorName,HardwareSerial *ptOutputSer,Logger *myLogger,MqttInterface* interface):Sensor(sensorName,ptOutputSer,myLogger,interface) {
	pression=0;
	temperature=0;
	lastTime=0;

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
void SensorBmp183::publish(){
	if(interface==NULL){
		return ;	 		
	}
	
	 _LOG_PRINT(M," publish start","");
	char topic[MQTT_TOPIC_MAX_SIZE];
	char payload[MQTT_PAYLOAD_MAX_SIZE];
	getData();	
	
	dtostrf(pression,7, 2,payload);
	_LOG_PRINT(D," pression tmp",payload);
	sprintf(topic,"%s/Pression atmos",sensorName.c_str());
	_LOG_PRINT(M," topic",topic);
	interface->publish(topic,payload);	
	dtostrf(SensorBmp183::temperature,5, 2,payload);
	sprintf(topic,"%s/temperature",sensorName.c_str());
	interface->publish(topic,payload);		

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

#undef NO_LOG
