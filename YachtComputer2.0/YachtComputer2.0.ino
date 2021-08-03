#include <SensorAnalog.h>
#include <SensorBmp183.h>
#include <Sensor.h>
//#include "ESP8266.h"
#include <Udpservices.h>
#include <CircBuf.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_HMC5883_U.h>

#include <Adafruit_BMP085.h>
#include <avr/interrupt.h>
#include <Wire.h>

#include <SensorBmp183.h>
#include <SensorHMC5883.h>
#include <Sensor.h>
#include <DigitalOutput.h>

// sensors
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 60000
#define TIME_INTERVAL_GIROUETTE 1000
#define TIME_INTERVAL3 150
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

#define X_OFFSET 5.87
#define Y_OFFSET 32.61
#define Z_OFFSET -10.98

//  digital output
#define RELAIS_SIZE 8
 //serials
 
#define ESP_SERIAL Serial3
#define LOG_SERIAL Serial
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200

#define TIME_INTERVAL 3000

//Wifi

#define SSID        "wifitelsur_MWAC"
#define PASSWORD    "0019C70D4077X"

#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416

//sensors pointers

SensorBmp183* mySensor;
SensorHMC5883* mySensor2;
SensorAnalog* mAnalogSensor;

//wifi service

Udpservices* mUdp;

//global variables

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float cap=0;
float k;


//ESP8266 wifi(Serial3,115200);
 
unsigned long nextTimeBaro=0;
unsigned long nextTimeAnemo=0;
unsigned long lastTimeRs232;
volatile unsigned long GanemoCount;
String inData;// input buffer rs232
int doneCount = 0;
unsigned long lastTime=0;
//digital output
int relais[RELAIS_SIZE]={46,47,48,49,50,51,52,53};
//int relais[RELAIS_SIZE]={13,47,48,49,50,51,52,53};
long timeout[RELAIS_SIZE]={1000,2000,0,0,0,0,0,0};

DigitalOutput mDoutput(RELAIS_SIZE,relais,timeout);
/**********************************************************************/
void setup() {

 /**********************************************************************/
  LOG_SERIAL.begin(LOG_BAUDRATE);//USB
  ESP_SERIAL.begin(ESP_BAUDRATE);//BT
  Serial2.begin(4800);//rs232
  LOG_SERIAL.println("Start");
 
 
 
  mAnalogSensor=new SensorAnalog(ANALOG_PIN,MAX_VALUE);

 
  // barometre
  
  mySensor = new SensorBmp183();
  if (mySensor->getError()) {
    LOG_SERIAL.println("ouverture SensorBmp183 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
    
  }
  delay(500);
  
  // girouette
  
  LOG_SERIAL.println("mySensor2");
  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    LOG_SERIAL.println("ouverture SensorHMC5883 " + mySensor2->getErrorMsg());
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    LOG_SERIAL.println(mySensor2->getSensorDetail());
  }
  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  LOG_SERIAL.println("fin setup anemometre ");
  delay(5000);
// WiFi Setup

  mUdp=new Udpservices(&ESP_SERIAL,&LOG_SERIAL);
  mUdp->init();
  delay(1000);

   lastTime=millis();
   LOG_SERIAL.print("setup end\r\n");
}


/**********************************************************************/
void loop() {
  
 /**********************************************************************/ 
 

 
  String msg;
  //read wifi
  msg=mUdp->readLine();
  if (msg.length()>0){
    LOG_SERIAL.println(msg);
    // mDoutput.decodeMsg(msg);// send message to digital output teemporary
  }
  // mDoutput.loop(); // for reset afeter time out
   
  // barometre
 if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
  // read barometre
   if (mySensor != NULL) {
      LOG_SERIAL.println("baro :");
     // mySensor->getData();
      sendMsg(mySensor->toNmea());
    }
 
  
  // anemo girouette
  
    if (mySensor2 != NULL) {
      LOG_SERIAL.println("girouette :");
      mySensor2->getData();
      sendMsg(mySensor2->toNmea());
      //LOG_SERIAL.println(mySensor2->toString());
     }
       // read barometre
   if (mySensor != NULL) {
      LOG_SERIAL.println("baro :");
      mySensor->getData();
      sendMsg(mySensor->toNmea());
    }
   lastTime=millis();
  }

  // read from port  ser2:

/*
  while (Serial2.available() > 0)
  {
    char recieved = Serial2.read();
    inData += recieved;
    //LOG_SERIAL.print("test: "+inData);
    if (recieved == '\n')
    {
         sendMsg(inData);
      }

      inData = ""; // Clear recieved buffer
    }

*/
  //LOG_SERIAL.println("loop end ");
}


void anemoInterupt() {
  GanemoCount++;
}

/**************************/

void sendMsg(String msg){
  LOG_SERIAL.println(msg);
  mUdp->println(msg);
  
}
