#include <Arduino.h>
#include"Logger.h"
#include "DigitalInput.h"


HardwareSerial* logSer;

char  machineName[]="EpsGirouette";
char  objectName[]="objectName";

//static Logger *myLogger;
 
void setup() {
 logSer=&Serial;
  logSer->begin(115200);
  
  //myLogger= new Logger(logSer);
   myLogger= new Logger(logSer,machineName,V);
 
  int i=999;
  unsigned char uc='g';
  unsigned int ui=15;
  float f=9.99;
  String s="a string";
  char charArray[]="charArray";
  char c='X';
  char Str[]="int";
  Serial.println();
 
_LOG_PRINT(D,"macro int",(i));
//LOG_PRINT(2,"macro int",(i,DEC));

  Serial.println();
  Serial.println();
 
  _LOG_PRINT(W,"int ",i);
  _LOG_PRINT(V,"unsigned char ",uc);
  _LOG_PRINT(M,"unsigned int ",ui);
  _LOG_PRINT(W,"char ",c);
  _LOG_PRINT(E,"float ",f);
  _LOG_PRINT(D,"String D",s);
  _LOG_PRINT(E,"String E",s);
  Serial.println();
  _LOG_PRINT(M,"charArray[] M",charArray);
  _LOG_PRINT(V,"charArray[] V",charArray);
 
}



void loop() {
  // put your main code here, to run repeatedly:

}

/*
void testprintLog(int level,char* sender,char* title,int value, int type){
    if (level>=0){
     headerprintLog(sender,title);
     Serial.print(value,type);
      //mPtLogSer->println(*tail);
    }
}

void headerprintLog(char *sender,char *title){
      Serial.print(sender);
      Serial.print("::");
      Serial.print(title);
      Serial.print(" :");
  }
  */
