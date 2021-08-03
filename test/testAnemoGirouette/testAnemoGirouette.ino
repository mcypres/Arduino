
#include <NmeaMwv.h>
#include <SensorGirouette.h>
#include <SensorHMC5883.h>
#include <SensorAnemometre.h>
#include"/home/marc/Arduino/libraries/Logger/Logger.h"
#include "tca.h"

char    machineName[] = "EpsGirouette";
Logger* myLogger;


HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;

NmeaMwv* myNmeaMwv;
SensorGirouette* myGirouette;

SensorHMC5883* sensor1;
SensorHMC5883* sensor2;
//SensorAnemometre* myAnemo;

void setup() {
    ptLogOutputSer = &Serial;
  ptLogOutputSer->begin(115200);
 // LevelValue{M=0,E=1,W=2,D=3,V=4}

  myLogger = new Logger(ptLogOutputSer, machineName,W);
   _LOG_PRINT(M, machineName, "Start");
  myNmeaMwv= new NmeaMwv(myLogger);
  Tca::tcaselect(0);
  sensor1=new SensorHMC5883(myLogger,0,0,0,0,0,0);
  if(sensor1==NULL){
    _LOG_PRINT(M, "Setup", "sensor1==NULL");
    
  }
  Tca::tcaselect(1);
  sensor2=new SensorHMC5883(myLogger,0,0,0,0,0,0);
  if(sensor1==NULL){
    _LOG_PRINT(M, "Setup", "sensor2==NULL");
  }
  myGirouette=new SensorGirouette(myLogger,sensor1,sensor2);
//myAnemo=new SensorAnemometre(myLogger);
 _LOG_PRINT(M, "Setup", "End");
}

void loop() {
  int heading;
  float speed;
  heading=myGirouette->getHeading();
  
  _LOG_PRINT(M, "heading :", heading);
  //speed=myAnemo->getSpeed();
  String msg=myNmeaMwv->toNmea( heading, 0);
  _LOG_PRINT(M, "myNmeaMwv", msg);
  delay(1000);
}
