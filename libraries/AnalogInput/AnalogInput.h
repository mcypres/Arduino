#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H
#include "Arduino.h"
#include "Sensor.h"
#include "SensorAnalog.h"
#define TIME_OUT 1500

class :AnalogInput public Sensor {
private:
	int * mPortList;
	int mNbPort;
	int mGroupNumber;
	//int * mportState;
	SensorAnalog * mSensorList;
	float refVoltage;
public:AnalogInput(HardwareSerial *ptOutputSer,int nbPort,int * portList,int GroupNumber,float refVoltage);
	String toString();
	void toNmea();
	void getData();
	unsigned long getValue();
	String read();
	//~DigitalIntput();
	
};
#endif
