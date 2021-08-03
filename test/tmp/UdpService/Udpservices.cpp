/*
 * Udpservices.cpp
 *
 *  Created on: 31 mai 2016
 *      Author: marc
 */

#include "Udpservices.h"

//namespace Udpservices {


Udpservices::Udpservices(HardwareSerial* EspSerial,
						HardwareSerial* LogSerial):
											mInputBuff(CIRC_BUFF_LENGH ){
	mEspSerial=EspSerial;
	mLogSerial=LogSerial;
	//mInputBuff =(CircBuff) new  CircBuff(CIRC_BUFF_LENGH );
}
bool Udpservices::init(String ssid,
			String password,
			String hostName,
			long hostPort,
			long remPort
			){
	 //Set wifi mode：
	   String command;
	  // mEspSerial->begin(ESP_BAUDRATE);
	  // mLogSerial->begin(LOG_BAUDRATE);
	   mLogSerial->println("setup Start");
	   
	   mLogSerial->println("sended AT+RST");
	   mEspSerial->println("AT+RST");
	   delay(5000);
	   LogReponce();					//Response：OK
	   
	  mLogSerial->println("sended AT+CWMODE=3");
	  mEspSerial->println("AT+CWMODE=3"); 			// softAP+station mode
	  delay(5000);
	  LogReponce();//Response：OK
	  
	  
	  //Connect to router
	  mLogSerial->println("sended AT+CWJAP=\""+ssid+"\",\""+password+"\"");
	  mEspSerial->println("AT+CWJAP=\""+ssid+"\",\""+password+"\""); // ssid and password of router
	  
	  
	  delay(5000);
	  LogReponce();						//Response：OK
	  //Query device’s IP
	  mLogSerial->println("sended  AT+CIFSR");
	  mEspSerial->println("AT+CIFSR");
	  delay(5000);
	  
	  // IP address of ESP8266 station
	  LogReponce();						//Response：+CIFSR:STAIP,"192.168.101.104" 
	  
	  //PC connects to the same router which ESP8266 connects to. Using a network tool (eg: ”NetAssist.exe”) on the computer to create UDP .
	  
	  
	  //Enable multiple connection
	  mLogSerial->println("sendedAT+CIPMUX=1");
	  mEspSerial->println("AT+CIPMUX=1");
	  delay(5000);
	  LogReponce();						//Response：OK
	  //Create a UDP transmission, for example, id is 4.
	  mLogSerial->println("sended AT+CIPSTART=4,\"UDP\",\""+hostName+"\","+hostPort+","+remPort+",0");
	  mEspSerial->println("AT+CIPSTART=4,\"UDP\",\""+hostName+"\","+hostPort+","+remPort+",0");
	  delay(5000);
	  LogReponce();						//Response：4,CONNECT OK
	  mLogSerial->println("sended AT+CIPSTATUS");
	  mEspSerial->println("AT+CIPSTATUS");
	  delay(5000);
	   LogReponce();
	  /*Note：
	  "192.168.101.110",8080 here is the remote ip and port of UDP transmission which create on PC in step 4;
	  1112 is the local port of ESP8266, user-define, if user does not define it, it will be a random value;
	  0 means destination peer entity of UDP will not change. For example, in this case, if another PC also creates a UDP entity and sends data to ESP8266 port 1112, ESP8266 can receive these data, but when we send data with command “AT+CIPSEND=4,X”, it will still be sent to the first PC. If this parameter is not 0, it will send to the new PC.
	  */

	  mLogSerial->println("**********setup end************");
	  return true;
}

	String Udpservices::readLine(){
		//mLogSerial->print("readLine available : ");
		//mLogSerial->println(mEspSerial->available());
		//+IPD,4,22:àaaaaaaaaaaaaaaaaaaaa
		String rep="";
		if (mEspSerial->available()>0){
	    	    String msg;
	    	    msg= mEspSerial->readStringUntil('\n');
	    	    //mLogSerial->print("received<<<<<<");
	    	   // mLogSerial->println(msg);
	    	    if(msg.startsWith("+IPD")==true){
	    	    	    rep=msg.substring(10);
	    	    	    mLogSerial->print("received<<<<<rep<");
	    	    	    mLogSerial->println(rep);

	    	    }
	    }
	    return rep;
	}
	void Udpservices::println(String str){
		mLogSerial->print("send>>>>>>");
	    mLogSerial->println(str);
		mEspSerial->print("AT+CIPSEND=4,");			
		mEspSerial->println(str.length());
	/*	while (mEspSerial->available()>0){
			String msg=mEspSerial->readStringUntil('\n');
			
			//mLogSerial->print(">>>>>>>>	received :");
			//mLogSerial->print(msg);
	*/
		//long startTime=millis()+SEND_DELAY;
		long startTime=millis()+2000;	

		while ((mEspSerial->available()>0)&&(startTime>millis())){}
		mLogSerial->println("----	available");
		char c;
		mLogSerial->println("----	received ---");
		do{
			c=mEspSerial->read();
			if (c!=-1){
				mLogSerial->print(c);
			}
		}
		while (c!='>'&&(startTime>millis()));
		mLogSerial->println("---");
		mEspSerial->print(str);
		
	}
	bool Udpservices::emptyBuffer(){
		String msg;
		if (mEspSerial->available()>0){
			msg=mEspSerial->readStringUntil('\n');
			//mLogSerial->print(">>>>>>>>received :");
			//mLogSerial->println(msg);
		}
		return true;
	}

	void Udpservices::LogReponce(){
		String msg="";
		//delay(1000);
		long startTime=millis();
		while (mEspSerial->available()==0 && startTime+TIMEOUT<millis()){}
		if (mEspSerial->available()==0){
			mLogSerial->println("no data received");
			mErr=true;
			return;
		}
		mLogSerial->println("data received");
		do{
			mLogSerial->println(mEspSerial->readStringUntil('\n'));
			//mLogSerial->write(mEspSerial->read());
			//msg=msg+mEspSerial->readString();
		}while(mEspSerial->available()>0);
		//int pos=mInputBuff.find('\n');
		
	}
Udpservices::~Udpservices() {
	//if (mInputBuff!=null) delete mInputBuff;
}

//} /* namespace Udpservices */
