

#include <SensorHMC5883.h>
#include"/home/marc/Arduino/libraries/Logger/Logger.h"


char    machineName[] = "EpsGirouette";
Logger* myLogger;


HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
SensorHMC5883* sensor1;
void setup() {
  ptLogOutputSer = &Serial;
  ptLogOutputSer->begin(115200);
 // LevelValue{M=0,E=1,W=2,D=3,V=4}

  myLogger = new Logger(ptLogOutputSer, machineName,W);
   _LOG_PRINT(M, machineName, "Start");

 sensor1=new SensorHMC5883(myLogger,0,0,0,0,0,0);
  if(sensor1==NULL){
    _LOG_PRINT(M, "Setup", "sensor1==NULL");
  }

    
   _LOG_PRINT(M, "SetUp End", "");
  delay(1000);  
 

}

void loop() {
  sensor1->getData();
  sensors_event_t event1=sensor1->getEvent();
  _LOG_PRINT(M, "event2 x :", event1.magnetic.x);
  _LOG_PRINT(M, "event2 y :", event1.magnetic.y);
  delay(1000);

}
