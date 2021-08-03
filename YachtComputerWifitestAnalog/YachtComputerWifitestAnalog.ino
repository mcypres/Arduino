#include <SensorHMC5883.h>
#include <SensorAnalog.h>
#include <SensorBmp183.h>
#include <Sensor.h>
#include "ESP8266.h"

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

SensorBmp183* mySensor;
SensorHMC5883* mySensor2;
SensorAnalog* mAnalogSensor;


int sensorPin = A0;    // select the input pin for the potentiometer
int sensorPin2 = A1;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float cap=0;
float k;
//Variables globales

ESP8266 wifi(Serial3,115200);
 
unsigned long nextTimeBaro=0;
unsigned long nextTimeAnemo=0;
unsigned long lastTimeRs232;
volatile unsigned long GanemoCount;
String inData;// input buffer rs232
int doneCount = 0;
unsigned long lastTime=0;
/**********************************************************************/
void setup() {

 /**********************************************************************/
  Serial.begin(115200);//USB
  //Serial3.begin(9600);//BT
  Serial2.begin(4800);//rs232
  Serial.println("Start");
 
 // Serial3.println("Start");
 
  mAnalogSensor=new SensorAnalog(ANALOG_PIN,MAX_VALUE);
  
  // barometre
  
  mySensor = new SensorBmp183();
  if (mySensor->getError()) {
    Serial.println("ouverture SensorBmp183 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
    
  }
  delay(500);
  
  // girouette
  
  Serial.println("mySensor2");
  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    Serial.println("ouverture SensorHMC5883 " + mySensor2->getErrorMsg());
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    mySensor2->getSensorDetail();
  }
  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  Serial.println("fin setup anemometre ");
  delay(5000);
// WiFi Setup
    Serial.println(wifi.restart());
   Serial.print("FW Version:");
    Serial.println(wifi.getVersion().c_str());
      
    if (wifi.setOprToStationSoftAP()) {
   
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");
        Serial.println(wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("multi err\r\n");
    }
    if(wifi.registerUDP(HOST_NAME ,HOST_PORT)){
       Serial.print("registerUDP ok\r\n");
    } else {
        Serial.print("registerUDP err\r\n");    
    }
    Serial.print("setup end\r\n");
}


/**********************************************************************/
void loop() {
  
 /**********************************************************************/ 
 // Serial.println("loop start receive");
 uint8_t buffer[200] = {0};
   uint32_t len = wifi.recv(buffer, sizeof(buffer), 1000);
    if (len > 0) {
        Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
    }
  /*    if (!wifi.unregisterUDP()) {
        Serial.print("unregister udp err\r\n");
    }*/
/*
      if (Serial3.available()>0){
      Serial.print("******** received :");
          
      Serial.print(Serial3.readStringUntil('\n'));
     
    }*/
 //Serial.println("loop end receive");
  String msg;
 if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
  //barometre
   if (mySensor != NULL) {
      Serial.println("baro");
      mySensor->getData();
      sendMsg(mySensor->toNmea());
    }
 
  
  // anemo girouette
 // if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){
    if (mySensor2 != NULL) {
      sendMsg(mySensor2->toNmea());
    //  Serial.println(mySensor2->toString());
     }
   lastTime=millis();
  }

  // read from port  ser2:

/*
  while (Serial2.available() > 0)
  {
    char recieved = Serial2.read();
    inData += recieved;
    //Serial.print("test: "+inData);
    if (recieved == '\n')
    {
         sendMsg(inData);
      }

      inData = ""; // Clear recieved buffer
    }

*/
//Serial.println("loop end ");
}


void anemoInterupt() {
  GanemoCount++;
}

/**************************/

void sendMsg(String msg){
 uint8_t buffer[200] = {0};
 //static uint8_t mux_id = 0;
 
     if (!wifi.registerUDP(HOST_NAME, HOST_PORT)) {
        Serial.println("register udp  err");
    }
    msg.toCharArray((char*)buffer,msg.length() +1);
    char *hello = (char*)&msg;
    Serial.println((char *)buffer);
    wifi.send((const uint8_t*)buffer, msg.length());
  /*  if (!wifi.unregisterUDP()) {
        Serial.print("unregister udp err\r\n");
    }*/
  /*  
  if (wifi.unregisterUDP(mux_id)) {
        Serial.print("unregister udp ");
        Serial.print(mux_id);
        Serial.println(" ok");
    } else {
        Serial.print("unregister udp ");
        Serial.print(mux_id);
        Serial.println(" err");
    }
    
    mux_id++;
    if (mux_id >= 5) {
        mux_id = 0;
    }*/
}
