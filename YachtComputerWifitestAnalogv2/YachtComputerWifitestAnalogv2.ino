#include <SensorHMC5883.h>
#include <SensorAnalog.h>
#include <SensorBmp183.h>
#include <Sensor.h>
//#define DEBUG
#define RECEIVE_USB
#define BAROMETRE
#define GIROUETTE 
#define RS232

#include "Udpservices.h"
#include "DigitalOutput.h"

#include <Adafruit_Sensor.h>

#include <Adafruit_HMC5883_U.h>

#include <Adafruit_BMP085.h>
#include <avr/interrupt.h>
#include <Wire.h>

#include <SensorBmp183.h>
#include <SensorHMC5883.h>
#include <Sensor.h>
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
//Wifi
#define SSID        "wifitelsur_MWAC"
#define PASSWORD    "0019C70D4077X"
//#define SSID        "marc-Aspire-E5-521G"
//#define PASSWORD    "mazel21"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416

// serial ports
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200
#define RS232_BAUDRATE 4800
#define ESP_SERIAL Serial3
#define RS232_SERIAL Serial2
#define LOG_SERIAL Serial


SensorBmp183* mySensor;
SensorHMC5883* mySensor2;
SensorAnalog* mAnalogSensor;
DigitalOutput * mDigit;
Udpservices* mUdp;

//Variables globales
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float cap=0;
float k;


int relais[]={46,47,48,49,50,51,52,53};
 
unsigned long nextTimeBaro=0;
unsigned long nextTimeAnemo=0;
unsigned long lastTimeRs232;
// compteur girouette
volatile unsigned long GanemoCount;

String inData;// input buffer rs232
int doneCount = 0;
unsigned long lastTime=0;

/**********************************************************************/
void setup() {

 /**********************************************************************/
  LOG_SERIAL.begin(LOG_BAUDRATE);     //USB
  RS232_SERIAL.begin(RS232_BAUDRATE); //rs232
  ESP_SERIAL.begin(ESP_BAUDRATE);     //WIFI



  
  LOG_SERIAL.println("Setup start");
  #ifdef WIFI
  mUdp=new Udpservices(&ESP_SERIAL,&LOG_SERIAL);
 // mUdp->init();
  mUdp->initTransparentUdp();
  #endif
  mAnalogSensor=new SensorAnalog(ANALOG_PIN,MAX_VALUE);

  // reception des sorties logique
  
  mDigit=new DigitalOutput(8,relais);

  // barometre
#ifdef BAROMETRE  
  LOG_SERIAL.println("Setup start BAROMETRE");
  mySensor = new SensorBmp183();
  if (mySensor->getError()) {
    LOG_SERIAL.println("ouverture SensorBmp183 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
    
  }
#endif  
  delay(500);
  
  // girouette
#ifdef GIROUETTE  
  LOG_SERIAL.println("Setup start GIROUETTE");
  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    LOG_SERIAL.println("ouverture SensorHMC5883 " + mySensor2->getErrorMsg());
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    LOG_SERIAL.println("girouette ok");
    LOG_SERIAL.println(mySensor2->getSensorDetail());
  }
#endif  
  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  LOG_SERIAL.println("fin setup anemometre ");
  delay(1000);

  LOG_SERIAL.print("setup end\r\n");
}


/**********************************************************************/
void loop() {
  
 /**********************************************************************/ 
 //LOG_SERIAL.println("start loop");
 //--------------------------------------------------------
 // reception
 #ifdef RECEIVE_WIFI
   String msgin=mUdp->read();
  if(msgin!=""){
    LOG_SERIAL.println(msgin);
    // positionement des ports logiques
    mDigit->write(msgin);
  }
  #endif
  #ifdef RECEIVE_USB
  if(LOG_SERIAL.available()>0){
    String msgin=LOG_SERIAL.readStringUntil('\n');
    if (msgin.length()>0){
      mDigit->write(msgin);
    }
  }
 
  #endif
 //LOG_SERIAL.println("end reading");
 //LOG_SERIAL.println("loop end receive");

 if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
  lastTime=millis();
 //------------------------------------------------------------ 
  //barometre
#ifdef BAROMETRE
   if (mySensor != NULL) {
     // LOG_SERIAL.println("baro");
      mySensor->getData();
  #ifdef WIFI     
     // mUdp->send(mySensor->toNmea());
  #endif
      LOG_SERIAL.println(mySensor->toNmea());
    }
#endif  
//-------------------------------------------------------------
  // anemo girouette
  delay(100);
#ifdef GIROUETTE 
    if (mySensor2 != NULL) {
      String msg;
      //LOG_SERIAL.println("mySensor2 start");
      mySensor2->getData();
      msg=mySensor2->toNmea();
     // LOG_SERIAL.print("mySensor2 send :");
 // #ifdef WIFI
//     mUdp->send(msg);
//  #endif
      LOG_SERIAL.println(msg);
    }
    else{
      LOG_SERIAL.println("mySensor2 = NULL");
    }
#endif
   lastTime=millis();
  }
//--------------------------------------------------------------
  // read from port  ser2:

#ifdef RS232
  if (RS232_SERIAL.available() > 0)
  {
    char received = RS232_SERIAL.read();
    if(received!=-1){
      inData += received;
      // LOG_SERIAL.print(received);
      if (received == '\n'){
          LOG_SERIAL.println(inData);
//#ifdef WIFI
         // mUdp->send(inData);
//          mUdp->transparentsend(inData);
//#endif
          inData = ""; // Clear received buffer
      }
    }

    
 }

#endif
//----------------------------------------------------------------
 // LOG_SERIAL.println("loop end ");
}


void anemoInterupt() {
  GanemoCount++;
}

/**************************/



