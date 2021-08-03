#include "DigitalInput.h"

#define NMEA_BLI_MSG "$PABLI,0,c,c,c,c,c,c,c,c*     "
//********************0123456789012345678901234567890
#define GROUP_OFFSET 7
#define VALUE_OFFSET 9
#define CHECKSUM_OFFSET 25
#define MSG_LENGTH_T_CHECKSUM 23

/****************************************************************************/

DigitalInput::DigitalInput(String sensorName,HardwareSerial *ptOutputSer,
											Logger *myLogger,
											int nbPort,
											int * portList,
											int GroupNumber,
											MqttInterface* interface):
												Sensor( sensorName,ptOutputSer,myLogger,interface){
	mNbPort=nbPort;
	mPortList=portList;
	mGroupNumber=GroupNumber%10;
	mportState=(int*)malloc(nbPort*sizeof(mportState[0])+1);
	for(int  i=0;i<mNbPort;i++){
		pinMode(mPortList[i], INPUT_PULLUP);
		mportState[i]=0;
	}
}

/****************************************************************************/

void DigitalInput::publish(){
	if(interface==NULL){
		return ;	 		
	}
	getData();
	for(int numPort=0;numPort<mNbPort;numPort++){
		char str[25];
		str[24]= 0;
		sprintf(str,"Entree Logique/%d",numPort);
		if(mportState[numPort]==1){
			
			interface->publish(str,"True");
			
	  	}
	  	else{
	  		interface->publish(str,"False");
	  	}	  	
	} 
}
	 
/***************************************************************************/

DigitalInput::~DigitalInput(){
	if (mportState!=NULL) delete mportState;
};

/**************************************************************************/

void DigitalInput::getData(){
  
  mPortsValues=0;  //?????
  _LOG_PRINT(V,F("mNbPort "),mNbPort);
  for(int numPort=0;numPort<mNbPort;numPort++){
	  
	_LOG_PRINT(V,F("mPortList[numPort]"),mPortList[numPort]);
  	mportState[numPort]=digitalRead(mPortList[numPort]);
	_LOG_PRINT(V,F("mportState[numPort]"),mportState[numPort]);     
  }  
  _LOG_PRINT(V,F("DigitalInput::getData() end"),"");
}

/****************************************************************************/

unsigned long DigitalInput::getValue(){
 	 getData();
	return mPortsValues;
}

/****************************************************************************/

String DigitalInput::toString(){ 
	getData();
	String rep="";
	for(int numPort=0;numPort<mNbPort;numPort++){
		rep =rep + "port :"+ mPortList[numPort] + " : " + mportState[numPort] + "\n";
	}
	return rep;
}
/****************************************************************************/
char* DigitalInput::toNmea(){
	_LOG_PRINT(V,F("DigitalInput::toNmea()"),"Start");
	static char msg[]=NMEA_BLI_MSG;
	//char msg[]=F("$PABLI,0,c,c,c,c,c,c,c,c*     ");
	int pos=VALUE_OFFSET;
	msg[GROUP_OFFSET]=mGroupNumber+'0';
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
		
	_LOG_PRINT(D,F("toNmea return"),msg);
	
	ptOutputSer->println(msg);
}
