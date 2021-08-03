/*
 * DigitalOutput.cpp
 *
 *  Created on: 14 juin 2016
 *      Author: marc
 */

#include "DigitalOutput.h"



DigitalOutput::DigitalOutput(int size,int *ports,long *delay) {
	mSize=size;
	mPorts=ports;
	mDelay=delay;
	mTimeout=new long(mSize);
	for(int  i=0;i<mSize;i++){
	  pinMode(ports[i], OUTPUT);
	  mTimeout[i]=0;
	  digitalWrite(ports[i], HIGH);
	}
}

void DigitalOutput::decodeMsg(String * msg){
	Serial.print("start decodeMsg( msg) ");
	Serial.println(*msg );
	int cmd=0;
	int nextLoop=0;
	mScanMsgPos=-1;
	do{
		cmd=getCommand(msg);
		Serial.print("getCommand( msg)= ");
		Serial.println(cmd);
		nextLoop=cmd;
		if (cmd>0 && cmd<9){
			cmd--;
		    digitalWrite(mPorts[cmd],LOW);
		    mTimeout[cmd]=millis()+mDelay[cmd];
		    Serial.print("at : ");
		    Serial.print(millis());
		    Serial.print("  ports ");
		    Serial.print(mPorts[cmd]);
		    Serial.println(" HIGH");

		  }
		 if (cmd<0 && cmd>-9){
			 cmd=-cmd-1;
		     digitalWrite(mPorts[cmd], HIGH);
		     Serial.print("at : ");
		     Serial.print(millis());
		     Serial.print("  ports ");
		     Serial.print(mPorts[cmd]);
		     Serial.println(" LOW");
		  }
		 Serial.print(" nextLoop = ");
		 Serial.println(nextLoop);

	}while( nextLoop!=0);

}
void DigitalOutput::loop(){
	for(int  i=0;mSize;i++){
		if (mTimeout[i]<millis()&&mTimeout[i]!=0){
		      digitalWrite(mPorts[i], HIGH);
		      Serial.print("at : ");
		       Serial.print(millis());
		      Serial.print("  ports ");
		      Serial.print(mPorts[i]);
		      Serial.println(" LOW");
		      mTimeout[i]=0;
		    }
	}
}
int DigitalOutput::getCommand(String * msg){
	int tmp=mScanMsgPos;
	Serial.print("mScanMsgPos msg = ");
	Serial.println(*msg);
	mScanMsgPos=msg->indexOf(',',tmp+1);
	Serial.print("mScanMsgPos = ");
	Serial.println(mScanMsgPos);
	Serial.print("tmp = ");
	Serial.println(tmp);

	if (mScanMsgPos>0){
		String cmd=msg->substring(tmp+1,mScanMsgPos);
		Serial.print("cmd = ");
		Serial.println(cmd);
		//tmp=mScanMsgPos;
		return cmd.toInt();
	}
	return 0;
}
DigitalOutput::~DigitalOutput() {
	if (mTimeout!=NULL) delete mTimeout;
}




