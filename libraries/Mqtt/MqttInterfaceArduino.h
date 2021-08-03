/*
 * MqttInterfaceArduino.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_ARDUINO_H_
#define MQTT_INTERFACE_ARDUINO_H_

#include <MqttInterface.h>
//#include <Logger.h>

class MqttInterfaceArduino : public MqttInterface{
public:
	MqttInterfaceArduino(HardwareSerial* ptEspSer,
							Logger* myLogger,
							const char* machineName,
							const char* netName,
							const int connectedPin);
	void publish(const char* topic, const char* payload);
	void connect();
	boolean isconnected();
private:
	HardwareSerial* ptEspSer;
	Logger* myLogger;
	boolean connected;
	const int connectedPin;
};

#endif
