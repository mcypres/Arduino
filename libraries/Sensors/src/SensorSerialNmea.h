
/*SensorSerialNmea.h
 * Marc Cypres
 * 10/10/2020
 * */
 
#ifndef SENSOR_SERIAL_NMEA_H
#define SENSOR_SERIAL_NMEA_H

#define SEP ','
#include <Sensor.h>

class SensorSerialNmea: public Sensor {
public:
	SensorSerialNmea(String sensorName,
					HardwareSerial *ptOutputSer,
					Logger *myLogger,
					MqttInterface* interface=NULL);
	void publish(char*  msg);
	void getData(){};
	String toString(){ return "";};

protected:
	virtual char* getTopicName(int i)=0;
	int topicNumber;
	char MsgName[8];
};
#endif
