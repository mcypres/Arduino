#include"Logger.h"
#include<string.h>

  String  machineName="EpsGirouette";
 char  objectName[11]="objectName";
Logger *myLogger;
HardwareSerial* logSer;
void setup() {
  logSer=&Serial;
  logSer->begin(115200);
  
  myLogger= new Logger(logSer,&machineName,2);
  int i=999;
  float f=9.99;
  String s="a string";
  char charArray[]="charArray";
  char c='X';
  char Str[]="int";
  myLogger->log(2,objectName,(char*)"int",i);
  myLogger->log(2,objectName,(char*)"float",f);
  myLogger->log(2,objectName,(char*)"String",s);
  myLogger->log(2,objectName,(char*)"charArray[]",charArray);
}



void loop() {
  // put your main code here, to run repeatedly:

}
