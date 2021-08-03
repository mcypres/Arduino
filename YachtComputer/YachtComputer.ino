#include <SensorHMC5883.h>
#include <SensorAnalog.h>
#include <SensorBmp183.h>
#include <Sensor.h>

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
#define TIME_INTERVAL1 1500
#define TIME_INTERVAL2 3000
#define TIME_INTERVAL3 5000
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

SensorBmp183* mySensor;
SensorHMC5883* mySensor2;
SensorAnalog* mAnalogSensor;

unsigned long lastTime;
unsigned long lastTimeRs232;
volatile unsigned long GanemoCount;
String inData;// input buffer rs232
int doneCount = 0;

void setup() {

  Serial.begin(9600);//USB
  Serial3.begin(9600);//BT
  Serial2.begin(4800);//rs232
  Serial.println("Start");
  Serial3.println("Start");
  mAnalogSensor=new SensorAnalog(ANALOG_PIN,MAX_VALUE);
  mySensor = new SensorBmp183();
  if (mySensor->getError()) {
    Serial.println("ouverture SensorBmp183 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
  }
  delay(2000);
  Serial.println("mySensor2");
  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN, 12345);
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
  Serial.println("setup fin");
  delay(5000);

}
/**********************************************************************/
void loop() {
  String msg;

  // Serial.println("loop start");
  if (lastTime + TIME_INTERVAL1 < millis() && doneCount == 0) {

    if (mySensor != NULL) {

      mySensor->getData();
      msg = mySensor->toNmea();
      Serial.println(msg);
      Serial3.println(msg);
    }
    doneCount++;
  }
  if (lastTime + TIME_INTERVAL2 < millis() && doneCount == 1) {
    if (mySensor2 != NULL) {

      // Serial.println(" mySensor2");
      mySensor2->getData();
      GanemoCount=mAnalogSensor->getValue();
      msg = mySensor2->toNmea();
      Serial.println(msg);
      Serial3.println(msg);
      // Serial.println(mySensor2->toString());
      // Serial.println(" mySensor2 fin");
    }
    doneCount++;
  }

  // read from port  ser2:


 if (lastTime + TIME_INTERVAL3 < millis() && doneCount == 2) {
   doneCount =0;
  while (Serial2.available() > 0)
  {
    char recieved = Serial2.read();
    inData += recieved;
    //Serial.print("test: "+inData);
    if (recieved == '\n')
    {
     

        Serial.print(inData);
        Serial3.print(inData);
        lastTime = millis();
        doneCount = 0;

      }

      inData = ""; // Clear recieved buffer
    }
  }

}


void anemoInterupt() {
  GanemoCount++;
}
