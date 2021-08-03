/*
 * MqttInterface.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_H_
#define MQTT_INTERFACE_H_
#include "Arduino.h"
#include "Logger.h"

#define START_CHAR "#"
#define END_CHAR #\0"
#define SEPA_CHAR "&"

#define MQTT_CONNECTED_MSG "$$$Connected$$$"
#define MQTT_DISNECTED_MSG "$$$Disnected$$$"
#define ESP_READY_MSG "$$$EspRady$$$"

class MqttInterface{
	public:
		MqttInterface(const char* machineName,const char* netName);
		virtual void publish(const char* topic, const char* payload)=0;
		boolean isConnected(){ 
			return connected;
		}
		virtual void connect()=0;
	protected:
		const char* netName;
		const char* machineName;
		boolean connected;
	private:
		xQueueHandle xQueuexxx;
		//MqttInterface(){}
};

#endif
