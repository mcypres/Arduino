
/* 
*	SensorSerialNmeaRmc.cpp  
* Marc Cypres
* 10/10/2020
* */


#include <SensorSerialNmeaRmc.h>



	SensorSerialNmeaRmc::SensorSerialNmeaRmc(String sensorName,
							HardwareSerial *ptOutputSer,
							Logger *myLogger,
							MqttInterface* interface):
								SensorSerialNmea(sensorName,
								ptOutputSer,
								myLogger,
								interface) {
	topicNumber=NB_FIELD;	
	strcpy(MsgName,"$GPRMC");
}
	char* SensorSerialNmeaRmc::getTopicName(int i){
		
		_LOG_PRINT(V, F("topicName[i] "), topicName[i] );
		return topicName[i];
	}

	
