/*
 * MqttInterfaceArduino.cpp
 *
 *  Created on:22/08/2020
 *      Author: marc
 */
// #define NO_LOG
#include <MqttInterfaceArduino.h>
#include <MqttInterface.h>


 MqttInterfaceArduino::MqttInterfaceArduino( HardwareSerial* ptEspSer,
 											Logger* myLogger,
 											const char* machineName,
 											const char* netName,
											const int connectedPin) : 
 												ptEspSer(ptEspSer),
 												myLogger(myLogger),
												connectedPin(connectedPin),
 												MqttInterface( machineName, netName){
 }
 
void MqttInterfaceArduino::publish( const char* topic, const char* payload){
	_LOG_PRINT(M," topic ",topic);
	_LOG_PRINT(M," payload ",payload);
	ptEspSer->print(START_CHAR);
	//_LOG_PRINT(V," START_CHAR ",START_CHAR);
	ptEspSer->print("p");
	//_LOG_PRINT(D," payload ","p");
	ptEspSer->print(SEPA_CHAR);	
	ptEspSer->print(netName);
	//_LOG_PRINT(D," netName ",netName);
	ptEspSer->print("/");
	//_LOG_PRINT(V," / ",'/');
	ptEspSer->print(machineName);
	//_LOG_PRINT(V," machineName ",machineName);
	ptEspSer->print("/");
	//_LOG_PRINT(V," / ",'/');
	ptEspSer->print(topic);
	//_LOG_PRINT(V," topic",topic);
	ptEspSer->print(SEPA_CHAR);	
	//_LOG_PRINT(V," SEPA_CHAR ",SEPA_CHAR);
	ptEspSer->print(payload);
	//_LOG_PRINT(V," payload",payload);
	ptEspSer->println(SEPA_CHAR);

}

boolean MqttInterfaceArduino::isconnected(){
	
	return (boolean)digitalRead( connectedPin);
}

void  MqttInterfaceArduino:: connect(){
}

//#undef NO_LOG

