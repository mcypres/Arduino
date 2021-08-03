

#include <SensorQMC5883.h>
#include"/home/marc/Arduino/libraries/Logger/Logger.h"
#include <Wire.h>

char    machineName[] = "EpsGirouette";
Logger* myLogger;


HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
SensorQMC5883* sensor1;

/********************************************/

void setup() {
  ptLogOutputSer = &Serial;
  ptLogOutputSer->begin(115200);
 // LevelValue{M=0,E=1,W=2,D=3,V=4}
  myLogger = new Logger(ptLogOutputSer, machineName,D);
   _LOG_PRINT(M, machineName, "Start");
 Wire.begin();
 sensor1=new SensorQMC5883(myLogger,0,0,0,0,0,0);
  if(sensor1==NULL){
    _LOG_PRINT(M, "Setup", "sensor1==NULL");
  }

 
   
  delay(1000);  
 
   
   _LOG_PRINT(M, "SetUp End", "");
}

void loop() {
   _LOG_PRINT(M, "getData Start", "");
  sensor1->getData();
     
   _LOG_PRINT(M, "getData End", "");
  sensors_event_t event1=sensor1->getEvent();
  _LOG_PRINT(M, "event2 x :", event1.magnetic.x);
  _LOG_PRINT(M, "event2 y :", event1.magnetic.y);
  delay(1000);

}
