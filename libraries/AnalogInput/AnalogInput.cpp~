#include "AnalogInput.h"

#define NMEA_BAI_MSG F("$PABAI,0,c,c,c,c,c,c,c,c*     ")
//********************0123456789012345678901234567890
#define GROUP_OFFSET 7
#define VALUE_OFFSET 9
#define CHECKSUM_OFFSET 25
#define MSG_LENGTH_T_CHECKSUM 23

AnalogInput::AnalogInput(HardwareSerial *ptOutputSer,int nbPort,int * portList,int GroupNumber,float refVoltage){
	mNbPort=nbPort;
	mPortList=portList;
	mGroupNumber=GroupNumber;
	mrefVoltage=refVoltage;
	//float *mportValue;
	mSensorList=malloc(nbPort*sizeof(SensorAnalog));
	for(int numPort=0;numPort<mNbPort;numPort++){
		mSensorList[numPort]= new SensorAnalog(portList[numPort],mrefVoltage);
      
     }
}

void AnalogInput::getData(){
  for(int numPort=0;numPort<mNbPort;numPort++){
  	mportValue[numPort]=digitalRead(mPortList[numPort]);
	    
  }
}

unsigned long AnalogInput::getValue(){
 	 getData();
	return mPortsValues;
}
void AnalogInput::toString(){ 
	getData();
	String rep="";
	for(int numPort=0;numPort<mNbPort;numPort++){
		rep =rep + "port: "+ mPortList[numPort] + " : " + mportState[numPort] + "\n";
	}
	return rep;
}
String AnalogInput::toNmea(){
	  char msg[]=NMEA_BAI_MSG;
	  int pos=VALUE_OFFSET;
	  msg[GROUP_OFFSET]=char(mGroupNumber);
	  getData();
	  for(int numPort=0;numPort<mNbPort;numPort++){
	  	  if(mportState[numPort]==true){
	  	  	  msg[pos]='T';
	  	  }
	  	  else{
	  	  	 msg[pos]='F';
	  	  }
	  	  pos+=2;
	  }
	  checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
	  ptOutputSer->println(msg);
}