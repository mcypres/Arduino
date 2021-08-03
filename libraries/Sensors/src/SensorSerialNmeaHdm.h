
/*SensorSerialNmeaRmc.h
 * Marc Cypres
 * 10/10/2020
 * */
 
#ifndef SENSOR_SERIAL_NMEA_HDM_H
#define SENSOR_SERIAL_NMEA_HDM_H

#include "SensorSerialNmea.h"
#define NB_FIELD 2
class SensorSerialNmeaHdm: public SensorSerialNmea {
public:
	SensorSerialNmeaHdm(String sensorName,
				HardwareSerial *ptOutputSer,
				Logger *myLogger,
				MqttInterface* interface=NULL);
	boolean publish(char* msg);
protected:
      	char* getTopicName(int i);

//$APHDM,313.1,M*33
/*
 *    1   313.1     cap
      2   M         typ Magnetique
      3  *33        checksum
*/
	//char topicName[NB_FIELD][6]={"Cap","Type"};

	char topicName[NB_FIELD][6]={"Cap",""};
};
#endif
