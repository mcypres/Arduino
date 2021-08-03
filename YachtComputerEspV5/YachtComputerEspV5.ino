
#define RECEIVE_USB
#define BAROMETRE
//#define GIROUETTE
#define RS232_2
#define RS232_3
#define WIFI
#define OPTOGROUP
#define ANALOG_GROUP
//#define LOGICAL_OUT
#define DEBUG


#include <SensorHMC5883.h>
#include <SensorAnalog.h>
#include <SensorBmp183.h>
#include <Sensor.h>
#include "DigitalOutput.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include "DigitalInput.h"
#include "SensorAnalogGroupInput.h"

#define WIFI_HEADER "***ESP"
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 60000
#define TIME_INTERVAL_GIROUETTE 10000
#define TIME_INTERVAL_OPTO_INPUT 10000
#define TIME_INTERVAL_ANALOG_GROUP 60000
#define TIME_INTERVAL3 150
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

#define FILTERCOUNT 30
/*
  #define X_OFFSET 5.87
  #define Y_OFFSET 32.61
  #define Z_OFFSET -10.98
*/
#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"
//#define SSID        "marc-Aspire-E5-521G"
//#define PASSWORD    "mazel21"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416
#define ESP_RESET_PIN  7

// serial ports
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200
#define RS232_2_BAUDRATE 4800
#define RS232_3_BAUDRATE 4800
//#define RS232_2_BAUDRATE 38400
//#define RS232_3_BAUDRATE 38400
//#define RS232_2_BAUDRATE 19200
//#define RS232_3_BAUDRATE 19200

#define RS232_SERIAL3 Serial3

#define ESP_SERIAL Serial1
#define RS232_SERIAL2 Serial2
#define LOG_SERIAL Serial


#define OPTOGROUP_NUMBER 1

#ifdef BAROMETRE
SensorBmp183* mySensor;
#endif
#ifdef GIROUETTE
SensorHMC5883* mySensor2;
#endif
SensorAnalog* mAnalogSensor;
#ifdef LOGICAL_OUT
DigitalOutput * mDigit;
#endif

//************************************************

//Variables globales

//************************************************

//Udpservices* mUdp;
//int sensorPin = A0;    // select the input pin for the potentiometer
//int sensorPin2 = A1;

static long timeCounter;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float cap = 0;
float k;
#ifdef OPTOGROUP
DigitalInput *  mOptoInputGroup;
int optoInput[] = {22, 23, 24, 25, 26, 27, 28, 29};
unsigned long mOptoInputInterval = 0;
#endif

int relais[] = {46, 47, 48, 49, 50, 51, 52, 53};


unsigned long nextOptoInput = 0;
unsigned long nextTimeBaro = 0;
unsigned long nextTimeAnemo = 0;
unsigned long nextTimeAnalogGroupInput = 0;
unsigned long lastTimeRs232;
// compteur girouette
volatile unsigned long GanemoCount;

String inData2;// input buffer rs232 port2
String inData3;// input buffer rs232 port3
String msgWifiIn = "";
String msgWifiOut = "";
int doneCount = 0;
unsigned long lastTime = 0;
boolean espReady = false;
//char wifiHeader[]= WIFI_HEADER;

SensorAnalogGroupInput * mInput;
SensorAnalogGroupInput * mInput2;

int filterCount = 0;
/**********************************************************************/
void setup() {

  /**********************************************************************/

  // serial
  // Serial.begin(LOG_BAUDRATE);
  LOG_SERIAL.begin(LOG_BAUDRATE);     //USB
  RS232_SERIAL2.begin(RS232_2_BAUDRATE); //rs232
  RS232_SERIAL3.begin(RS232_3_BAUDRATE); //rs232
  ESP_SERIAL.begin(ESP_BAUDRATE);     //WIFI
  LOG_SERIAL.println("Setup start");




  int pos = 0;
  boolean ledBuildIn = false;


  pinMode(ESP_RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //reset esp8266
  //____________
#ifdef WIFI
  digitalWrite(ESP_RESET_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(ESP_RESET_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(ESP_RESET_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  LOG_SERIAL.println("waiting esp start");
  
  while (espReady == false) {
    if (ledBuildIn) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledBuildIn = false;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      ledBuildIn = true;
    }
    //ledBuildIn!=ledBuildIn;
    if (ESP_SERIAL.available() > 0) {
      // get the new byte:
      char inChar = (char)ESP_SERIAL.read();
      LOG_SERIAL.print(inChar);
      // add it to the msgin:
      msgWifiIn += inChar;
      if (inChar == '\n' && msgWifiIn.length() > 0) {
        if (msgWifiIn.indexOf("Broadcasting") > 0) {
          espReady = true;
        }
        msgWifiIn = "";
      }
    }
    //delay(100);


  }
  msgWifiIn = "";
  LOG_SERIAL.println("\n esp started");
  digitalWrite(LED_BUILTIN, LOW);
#endif

  // initialisation des entree logiques
  //______________________________________

#ifdef OPTOGROUP
  mOptoInputGroup = new  DigitalInput(sizeof(optoInput) / sizeof(int), optoInput, OPTOGROUP_NUMBER);
#endif
  // initialisation des entree analogiques
  //______________________________________
#ifdef ANALOG_GROUP
  const int pinNumber[] = {8, 9, 10, 11};
  const int pinNumber2[] = {12, 13, 14, 15};
  const float scale[] = {66, 66, 66, 66};
  const float scale2[] = {0.653, 1.2995, 1.2995, 1.2995};
  const int offset[] = {512, 512, 512, 512};
  const int offset2[] = {0, 0, 0, 0};
  const float refVoltage = 5000; // in mV
  const float refVoltage2 = 20855; // in mV !!!!!!!!!!!!!!!!!!!

  // const int refSource=DEFAULT;
  mInput = new SensorAnalogGroupInput('A', INT, pinNumber, scale, offset, refVoltage, DEFAULT);
  mInput2 = new SensorAnalogGroupInput('A', VOL, pinNumber2, scale2, offset2, refVoltage2, EXTERNAL);
#endif
  // reception des sorties logique
  //_______________________________
#ifdef LOGICAL_OUT
  mDigit = new DigitalOutput(8, relais);
#endif
  // barometre bmp180
  //__________

#ifdef BAROMETRE
  LOG_SERIAL.println("Setup start BAROMETRE");
  mySensor = new SensorBmp183();
  if (mySensor->getError()) {
    LOG_SERIAL.println("erreur ouverture SensorBmp183 " + mySensor->getErrorMsg() + "*******");
    delete mySensor;
    mySensor = NULL;
    LOG_SERIAL.println("probleme ouverture SensorBmp183 " );
  }
  else {
    LOG_SERIAL.println("ouverture SensorBmp183 OK " );
  }
#endif
  delay(500);

  // girouette
  //__________

#ifdef GIROUETTE
  LOG_SERIAL.println("Setup start GIROUETTE");
  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN, X_OFFSET, Y_OFFSET, Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    LOG_SERIAL.println("ouverture SensorHMC5883 " + mySensor2->getErrorMsg());
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    LOG_SERIAL.println("girouette ok");
    LOG_SERIAL.println(mySensor2->getSensorDetail());
  }

  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  LOG_SERIAL.println("fin setup anemometre ");
  delay(1000);
#endif
  timeCounter = 0;
  LOG_SERIAL.print("setup end\r\n");



#ifdef OPTOGROUP
LOG_SERIAL.print("OPTOGROUP  TIME_INTERVAL_OPTO_INPUT :");
LOG_SERIAL.println(TIME_INTERVAL_OPTO_INPUT);
#endif
#ifdef ANALOG_GROUP
LOG_SERIAL.print("ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP :");
LOG_SERIAL.println(TIME_INTERVAL_ANALOG_GROUP);
#endif
#ifdef RECEIVE_USB
LOG_SERIAL.println("RECEIVE_USB");
#endif
#ifdef BAROMETRE
LOG_SERIAL.println("BAROMETRE");
#endif
#ifdef GIROUETTE
LOG_SERIAL.println("GIROUETTE");
#endif
#ifdef RS232_2
LOG_SERIAL.print("RS232_2 ");
LOG_SERIAL.println(RS232_2_BAUDRATE);
#endif
#ifdef RS232_3
LOG_SERIAL.print("RS232_3 ");
LOG_SERIAL.println(RS232_3_BAUDRATE);
#endif
#ifdef WIFI
LOG_SERIAL.println("WIFI");
#endif
}
/**********************************************************************/
void loop() {

  /**********************************************************************/
  //LOG_SERIAL.println("start loop");
  //--------------------------------------------------------
  // reception
#ifdef WIFI
  if (ESP_SERIAL.available() > 0) {

    // get the new byte:
    char inChar = (char)ESP_SERIAL.read();
    LOG_SERIAL.print(inChar);
    // add it to the msgin:
    msgWifiIn += inChar;
    if (inChar == '\n' && msgWifiIn.length() > 0) {
#ifdef LOGICAL_OUT
      // positionement des ports logiques
      mDigit->write(msgWifiIn);

      LOG_SERIAL.print("*************IN***IN***************");
      LOG_SERIAL.println(msgWifiIn);
#endif
      msgWifiIn = "";
    }
  }
#endif

  //______________________________________________________

#ifdef RECEIVE_USB
  if (LOG_SERIAL.available() > 0) {

    // get the new byte:
    char inChar = (char)LOG_SERIAL.read();
    LOG_SERIAL.print(inChar);
    // add it to the msgWifiOut:
    msgWifiOut += inChar;
    if (inChar == '\n' && msgWifiOut.length() > 0) {
      ESP_SERIAL.println(msgWifiOut);
#ifdef RS232_2
      RS232_SERIAL2.println(msgWifiOut);
#endif
#ifdef RS232_3
      RS232_SERIAL3.println(msgWifiOut);
#endif
      // LOG_SERIAL.println(msgWifiOut);
      msgWifiOut = "";
    }
  }

#endif
  //_____________________________________________________
  //LOG_SERIAL.println("end reading");
  //LOG_SERIAL.println("loop end receive");

  if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {
    LOG_SERIAL.print("******** nbLoop :");
    LOG_SERIAL.println(timeCounter);
    timeCounter = 0;
    lastTime = millis();
    //------------------------------------------------------------

    //barometre
    //__________

#ifdef BAROMETRE
    // LOG_SERIAL.println("start baro");
    if (mySensor != NULL) {
      // LOG_SERIAL.println("baro");
      mySensor->getData();
      String msg = mySensor->toNmea();
#ifdef WIFI
      ESP_SERIAL.println(msg);
#endif
      LOG_SERIAL.println(msg);
    }
    /*test gps

      ESP_SERIAL.println(toNmeaRMC());
      LOG_SERIAL.println(toNmeaRMC());*/
#endif
    //-------------------------------------------------------------
    // anemo girouette
    //________________
    //delay(100);
#ifdef GIROUETTE
    if (mySensor2 != NULL) {
      String msg;
      //LOG_SERIAL.println("mySensor2 start");
      mySensor2->getData();
      msg = mySensor2->toString();
      //msg=mySensor2->toNmea();
      // LOG_SERIAL.print("mySensor2 send :");
      // #ifdef WIFI
      //     mUdp->send(msg);
      //  #endif
      LOG_SERIAL.println(msg);
    }
    else {
      LOG_SERIAL.println("mySensor2 = NULL");
    }
#endif
    // lastTime=millis();
  }
  else {
    timeCounter++;
  }

  //___________________________________________________________

  // read from port  ser2:
  //_____________________

#ifdef RS232_2
  if (RS232_SERIAL2.available() > 0)
  {
    char received = RS232_SERIAL2.read();
    if (received != -1) {
      inData2 += received;
      // LOG_SERIAL.print(received);
      if (received == '\n') {
        // LOG_SERIAL.println(inData2);
        ESP_SERIAL.println(inData2);
        inData2 = ""; // Clear received buffer
      }
    }
  }
#endif
  //___________________________________________________________

  // read from port  ser3:
  //_____________________

#ifdef RS232_3
  if (RS232_SERIAL3.available() > 0)
  {
    char received = RS232_SERIAL3.read();
    if (received != -1) {
      inData3 += received;
      LOG_SERIAL.print(received);
      if (received == '\n') {
        //       if(filterCount>=FILTERCOUNT){
        // LOG_SERIAL.println(inData3);
        ESP_SERIAL.println(inData3);
        //         filterCount=0;

        //       }
        //       filterCount++;
        inData3 = ""; // Clear received buffer
      }
    }
  }
#endif

  // lecture des entrees logiques
  //______________________________________

#ifdef OPTOGROUP
  if (mOptoInputInterval + TIME_INTERVAL_OPTO_INPUT < millis()) {
    String msg = mOptoInputGroup->toNmea();
    ESP_SERIAL.println(msg);
    LOG_SERIAL.println(msg);
    mOptoInputInterval = millis();
  }
#endif
  // lecture des entrees analogiques
  //______________________________________
#ifdef ANALOG_GROUP
  if (nextTimeAnalogGroupInput + TIME_INTERVAL_ANALOG_GROUP < millis()) {
    String msg = mInput->toNmea();
    String msg2 = mInput2->toNmea();
    ESP_SERIAL.println(msg);
    ESP_SERIAL.println(msg2);
    LOG_SERIAL.println(msg);
    LOG_SERIAL.println(msg2);

    nextTimeAnalogGroupInput = millis();
  }
#endif

  //----------------------------------------------------------------
  // LOG_SERIAL.println("loop end ");
}

// interup routine pour Girouette
//_______________________________

#ifdef GIROUETTE
void anemoInterupt() {
  GanemoCount++;
}
#endif

/**************************/
// pour test

String toNmeaRMC() {
  String tst = "$PERMC,160023.65,A,40.07,S,77.21,W,5.5,185,090317,7.3,E*7A";
  return tst;
}



