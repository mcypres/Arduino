#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H
#include "Arduino.h"
#include "Sensor.h"
#include"Logger.h"
#define TIME_OUT 1500

class DigitalInput: public Sensor {

public:
	DigitalInput(String sensorName,HardwareSerial *ptOutputSer,Logger *myLogger,int nbPort,int * portList,int GroupNumber,MqttInterface* interface=NULL);
	String toString();
	char* toNmea();	
	void publish();
	void getData();
	unsigned long getValue();
	String read();
	~DigitalInput();
	
private:
	//Logger *myLogger; est deja ds sensor
	int * mPortList;
	int mNbPort;
	int mGroupNumber;
	int * mportState;
	unsigned long mPortsValues=0;
	//HardwareSerial *ptOutputSer;	est deja ds sensor
};
#endif
