 /*
 * SensorAnalogGroupInput.cpp
 *
 *  Created on: 15/08/2017
 *      Author: marc
 */
 #include "SensorAnalogGroupInput.h"

 	SensorAnalogGroupInput::SensorAnalogGroupInput(HardwareSerial *ptOutputSer,
 							Logger *myLogger,
 							const char unit,
 							const NMEA_TYPE type,
 							const int *pinNumber,
 							const float *fullScale,
 							const int *offset,
 							const float refVoltage,
 							const int refSource):Sensor(ptOutputSer,myLogger){
		 
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
		analogReference(mRefSource);
		for(int i=0;i<NBENTRY;i++){
			int val;
			val = analogRead(mPinNumber[i]);// read the input pin
		}
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
		_LOG_PRINT(D,"lecture corrigee ",current);
		_LOG_PRINT(D,"mRefVoltage =",mRefVoltage);
		_LOG_PRINT(D," input mv =",(current*mRefVoltage));
		current=((current*mRefVoltage)/mFullScale[entry]);
		_LOG_PRINT(D,"current =",current);
		return current;
	 }
	 	 
	 /*************************************************************************/
	 

	 String SensorAnalogGroupInput::toString(){}
	 	 
	 /*************************************************************************/
	 

	  void SensorAnalogGroupInput::getData(){}
 
 


 


