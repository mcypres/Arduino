 /*
 * SensorAnalogGroupInput.cpp
 *
 *  Created on: 15/08/2017
 *      Author: marc
 */
 #define NO_LOG
 #include "SensorAnalogGroupInput.h"

 	SensorAnalogGroupInput::SensorAnalogGroupInput(String sensorName,HardwareSerial *ptOutputSer,
 							Logger *myLogger,
 							const char unit,
 							const NMEA_TYPE type,
 							const int *pinNumber,
 							const float *fullScale,
 							const int *offset,
 							const float refVoltage,
 							const int refSource,
 							MqttInterface* interface):
 								Sensor(sensorName,ptOutputSer,myLogger,interface) {
		 
		 for(int i=0;i<NBENTRY;i++){ 
		 	 pinMode(pinNumber[i], INPUT);
		 	 mFullScale[i]=fullScale[i];
		 	 mOffset[i]=offset[i];
		 	 mPinNumber[i]=pinNumber[i];
		 	 mRefVoltage=refVoltage/NB_ADC_STEPS;
		 }
		 mType=type;
		 mRefSource=refSource;
		 analogReference(mRefSource);
	 }      
	 
	 /*************************************************************************/
	 
	 char* SensorAnalogGroupInput::toNmea(){
	 	 _LOG_PRINT(D," SensorAnalogGroupInput","toNmea");
		/*analogReference(mRefSource);
		for(int i=0;i<NBENTRY;i++){
			int val;
			val = analogRead(mPinNumber[i]);// read the input pin
		}*/
		
		getData();
		
		 switch (mType){
		 case INT:
		 	 {
		 	 	 static char msgI[]=NMEA_INT_MSG;
		 	 	 buildNmeaMsg( msgI);
		 	 	 return msgI;
		 	 }
		 	 break;
		 case VOL:
		 	 {
		 	 	static char msgV[]=NMEA_VOL_MSG;
		 	 	 buildNmeaMsg(msgV);
		 	 	 return msgV;
		 	 }
		 	 break;
		 default:
		 	 _LOG_PRINT(M," invalid type" ,mType);
		 	 return NULL;
		 }
	
	 }
/*************************************************************************/	
	  
	 void SensorAnalogGroupInput::publish(){
		 
;
	if(interface==NULL){
		_LOG_PRINT(E,"interface==NULL","");
		return ;	 		
	}
	
		 		
	_LOG_PRINT(D," SensorAnalogGroupInput","pulish");
	analogReference(mRefSource);
	for(int i=0;i<NBENTRY;i++){
		char payload[MQTT_PAYLOAD_MAX_SIZE];
		char topic[MQTT_TOPIC_MAX_SIZE];
		topic[MQTT_TOPIC_MAX_SIZE-1]= 0;
		delay(10);
		float current=getEntryValue(i);
		_LOG_PRINT(M," sensorName" ,sensorName);
		_LOG_PRINT(M," mType" ,mType);
		dtostrf(current,6, 3,payload);
		switch (mType){
		 case INT:
		 	 {
				 sprintf(topic,"%s/Ampermetre/%d",sensorName.c_str(),i);
				 _LOG_PRINT(M," topic int" ,topic);
		 	 }
		 	 break;
		 case VOL:
		 	 {				 
				sprintf(topic,"%s/Voltmetre/%d",sensorName.c_str(),i);
				_LOG_PRINT(M," topic vol" ,topic);
			}
		 	 break;
		 }
		
		//dtostrf(current,6, 3,payload);
		_LOG_PRINT(M," topic" ,topic);
		_LOG_PRINT(M," payload",payload);
		interface->publish(topic,payload);

	}
}

/*************************************************************************/
	 

	 void SensorAnalogGroupInput::buildNmeaMsg(char * msg){
	 	             
	 	 int blockPos=FIRST_BLOCK_OFFSET;
		 for(int i=0;i<NBENTRY;i++){
		 	 delay(10);
		 	 float current=getEntryValue(i);
		 	 if(current<0){
		 	 	 msg[blockPos+SIGNE_OFFSET]='-';
			} 
			 char tmp[10];
			 dtostrf(abs(current),5, 3,tmp);
			 int len=strlen(tmp);
			 if (len>5) len=5;
			 _LOG_PRINT(V,"tmp",tmp);
			 strncpy(msg+blockPos+VALUE_OFFSET,tmp,len);
			 blockPos+=BLOCK_LENGTH;
		 }
		 checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
		 _LOG_PRINT(M,"msg",msg);
		 ptOutputSer->println((String)msg);
	 }
		 
	 /*************************************************************************/
	 
 
	 float SensorAnalogGroupInput::getEntryValue(int entry){
		int val = 0;
		float current;
		char saveChar;
		//mLastRead[entry] = analogRead(mPinNumber[entry]);// read the input pin
		//mLastRead[entry] = analogRead(mPinNumber[entry]);// read the input pin
		
		_LOG_PRINT(D,"pin ",mPinNumber[entry]);
		_LOG_PRINT(D," mLastRead[entry] =",mLastRead[entry]);
	
		mLastRead[entry] = analogRead(mPinNumber[entry]);// read the input pin

		if (mLastRead[entry]>0){ 
			current=mLastRead[entry]-mOffset[entry]+0.5;
		}
		else{ 
			current=mLastRead[entry]-mOffset[entry];
		}
		_LOG_PRINT(M,"lecture corrigee ",current);
		_LOG_PRINT(M,"mRefVoltage =",mRefVoltage);
		_LOG_PRINT(M," input mv =",(current*mRefVoltage));
		current=((current*mRefVoltage)/mFullScale[entry]);
		_LOG_PRINT(M,"current =",current);
		return current;
	 }
	 	 
	 /*************************************************************************/
	 

	 String SensorAnalogGroupInput::toString(){}
	 	 
	 /*************************************************************************/
	 

void SensorAnalogGroupInput::getData(){
	analogReference(mRefSource);
	for(int i=0;i<NBENTRY;i++){
		int val;
		val = analogRead(mPinNumber[i]);// read the input pin
	}
	  
}
 
 


#undef NO_LOG


