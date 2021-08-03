
/* 
*	SensorSerial.cpp  
* Marc Cypres
* 10/10/2020
* */


#include "SensorSerialNmea.h"


	SensorSerialNmea::SensorSerialNmea(String sensorName,
							HardwareSerial *ptOutputSer,
							Logger *myLogger,
							MqttInterface* interface):
								Sensor(sensorName,
								ptOutputSer,
								myLogger,
								interface) {

									
}
	void SensorSerialNmea::publish(char* msg){
		char* ptStart;
		char* ptEnd;
		char topic[MQTT_TOPIC_MAX_SIZE];
		char payload[MQTT_PAYLOAD_MAX_SIZE];
		char ptChk[3];
		if(interface==NULL){
			_LOG_PRINT(D, F("**** INTERFACE NIDED *******"), "" );
			return;
		}
		
		_LOG_PRINT(M, F("publish msg ="), msg );

		// if (strncmp(msg,"$GPRMC",6)==0) {
		 int lg=strchr(MsgName,'\0');
		 if (strncmp(msg,MsgName,lg)==0) {
		 	_LOG_PRINT(M, F("bad entete "), "" );
		 	return false;
		 }
		 //the  NMEA message es of type RMC
		 // check checksum
		ptEnd=strchr(msg, '*');
		ptStart=strchr(msg, '$');
		int ptChkCompuInt;
		checksum(ptStart, ptChk, (int)(ptEnd-ptStart)-1,&ptChkCompuInt);
		int ptChkMsgInt= (int)strtol(ptEnd+1, NULL, 16);
		if(ptChkMsgInt!=ptChkCompuInt){
			_LOG_PRINT(M, F("bad checksum msg "), ptChkMsgInt );
			_LOG_PRINT(M, F("checksum Computed "), ptChkCompuInt );
			_LOG_PRINT(M, F("bad char checksum msg "), ptEnd );
			_LOG_PRINT(M, F("bad checksum ptEnd-msg-2 "), (int)(ptEnd-ptStart)-1 );

			return true;
		}
		// the message is valid
		
		ptStart=strchr(msg,SEP);
		ptEnd=strchr(ptStart+1,SEP);
		// while (ptEnd!=NULL){
		_LOG_PRINT(V, F("topicNumber "), topicNumber );
		for(int i=0;i<topicNumber && ptEnd!=NULL;i++){
			if(strlen(getTopicName(i))>0){
				int length=(int)(ptEnd-ptStart)-1;
				_LOG_PRINT(V, F("length "), length );
				if (length>0){
					strncpy(payload,ptStart+1,length);
					payload[length]='\0';
					sprintf(topic,"%s/%s",sensorName.c_str(),getTopicName(i));
					_LOG_PRINT(V, F("topic "), topic );
					_LOG_PRINT(V, F("payload "), payload );
					interface->publish(topic,payload);
				}
				ptStart=ptEnd;
				ptEnd=strchr(ptStart+1,SEP);
			}
		}	
	}
	
	
