#include "DigitalOutput.h"

#define NMEA_MPT_MSG "$PAMPR,00000.00,hPa,000.00, C*     "
//********************0123456789012345678901234567890
DigitalOutput::DigitalOutput(String sensorName,Logger *myLogger,int nbPort,int * portList){
	this->sensorName= sensorName;
	this->myLogger=myLogger;
	mNbPort=nbPort;
	mPortList=portList; 
	mTimeout=new long(nbPort);
	for(int  i=0;i<mNbPort;i++){
      pinMode(mPortList[i], OUTPUT);
      mTimeout[i]=0;
      digitalWrite(mPortList[i], HIGH);
     }
}

void DigitalOutput::write(String msg){
  
  int numRelais=0;
  //Serial.print("writeRelais(const char msg[]:");
  //Serial.println(msg);
  _LOG_PRINT(D,"writeRelais(const char msg[]",msg);
  
  for(int i=0;i<msg.length() && numRelais<mNbPort;i++){
  	  

  	//Serial.print(" msg[");
    //Serial.print(i);
    _LOG_PRINT_TAIL(D," msg[",i,"");
    //Serial.print("]=");
    //Serial.println(msg.charAt(i));
    _LOG_PRINT(D,"]=",msg.charAt(i));


	if(msg.charAt(i)=='0'){
      //Serial.print("Relais[");
      //Serial.print(numRelais);
     // Serial.println("]=0");
      _LOG_PRINT_TAIL(D,"Relais[ ",numRelais,"]=0\n");
      
      digitalWrite(mPortList[numRelais],HIGH);
      numRelais++;
    }
   if(msg.charAt(i)=='1'){
  	   
      //Serial.print("Relais[");
      //Serial.print(numRelais);
      //Serial.println("]=1");
      _LOG_PRINT_TAIL(D,"Relais[",(i),"]=1");

      digitalWrite(mPortList[numRelais],LOW);
      mTimeout[numRelais]=millis()+TIME_OUT;
      numRelais++;
    }

  }
}
void DigitalOutput::refresh(){
   for(int  i=0;i<mNbPort;i++){
      if (mTimeout[i]<millis()&&mTimeout[i]!=0){
        digitalWrite(mPortList[i], HIGH);
        
        
        //Serial.print("at : ");
       // Serial.print(millis());
        _LOG_PRINT_TAIL(D,"at : ",millis(),"");
        
       // Serial.print("  mPortList ");
       // Serial.print(mPortList[i]);
       // Serial.println(" LOW");
        _LOG_PRINT_TAIL(D,"  mPortList ",mPortList[i]," LOW");
        mTimeout[i]=0;
      }
   }
}
   DigitalOutput::~DigitalOutput(){
   	  if (mTimeout != NULL){
   	  	  delete mTimeout;
   	  }
	};


