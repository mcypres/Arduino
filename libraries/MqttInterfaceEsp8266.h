/*
 * MqttInterfaceEsp8266.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_ESP8266_H_

class MqttInterfaceEsp8266 :pulic MqttInterface{
		MqttInterface(String machineName,String netName);
		boolean publish(const char* topic, const char* payload);
		virtual boolean connect();
}

#ENDIF
