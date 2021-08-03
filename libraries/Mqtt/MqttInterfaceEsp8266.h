/*
 * MqttInterfaceEsp8266.h
 *
 *  Created on:22/08/2020
 *      Author: marc
 */

#ifndef MQTT_INTERFACE_ESP8266_H_
#define MQTT_INTERFACE_ESP8266_H_
#ifndef __AVR_ATmega2560__

#include </home/shares/public/libraries/Mqtt/MqttInterface.h>
#include <PubSubClient.h>

#include <ESP8266WiFi.h>
#define MSG_SIZE 200
#define TOPIC_SIZE 200


enum StateWaiting {
  start,
  commande,
  endTopic,
  endMessage
  
};

	
class MqttInterfaceEsp8266 :public MqttInterface{
public:
	MqttInterfaceEsp8266(HardwareSerial* ptEspSer,
								Logger* myLogger,
								const char* machineName,
								const char* netName);
	void publish(const char* topic, const char* payload);
	void publishSerial(String* ptMsg);
	void readSerial();
	void connect();
	void loop();
	int connected();
	void setServer(const char* mqtt_server,const int port );
	void setCallback(void callback(char* topic, byte* payload, unsigned int length));
private:
	WiFiClient espClient;
	PubSubClient* MQTTclient;
	HardwareSerial* ptOutputSer;
	Logger* myLogger;
	StateWaiting state;
	char buffMsg[MSG_SIZE];
	char buffTopic[TOPIC_SIZE];
	int posTopic,posMsg;
	char cmd;
};
#endif
#endif
