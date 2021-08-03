 
#define RECEIVE_USB
#define BAROMETRE
//#define GIROUETTE
#define RS232_2
#define RS232_3
#define WIFI
//#define OPTOGROUP
//#define ANALOG_GROUP
//#define LOGICAL_OUT
//#define DEBUG

#include"Logger.h"

#ifdef GIROUETTE
#include <Adafruit_HMC5883_U.h>
#include <SensorHMC5883.h>
#endif
#ifdef ANALOG_GROUP
#include <SensorAnalog.h>
#include "SensorAnalogGroupInput.h"
#endif
#ifdef BAROMETRE
#include <SensorBmp183.h>
#endif

//#include <Sensor.h>
#ifdef LOGICAL_OUT
#include "DigitalOutput.h"
#endif
#include <Adafruit_Sensor.h>
//#include <Adafruit_BMP085.h>
#include <avr/interrupt.h>
#ifdef OPTOGROUP
#include "DigitalInput.h"
#include "SensorAnalogGroupInput.h"
#endif

#include <Wire.h>

#define WIFI_HEADER F("***ESP")
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 60000
#define TIME_INTERVAL_GIROUETTE 20000
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

//#define RS232_2_BAUDRATE 4800
//#define RS232_3_BAUDRATE 4800
#define RS232_3_BAUDRATE 115200
#define RS232_2_BAUDRATE 38400
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
#ifdef ANALOG_GROUP
SensorAnalog* mAnalogSensor;
#endif
#ifdef LOGICAL_OUT
DigitalOutput * mDigit;
#endif

//************************************************

//Variables globales

//************************************************

char    machineName[]="YcMain";

//Udpservices* mUdp;
//int sensorPin = A0;    // select the input pin for the potentiometer
//int sensorPin2 = A1;

static long timeCounter;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
//float cap = 0;
//float k;
#ifdef OPTOGROUP
DigitalInput *  ptOptoInputGroup;
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


#ifdef ANALOG_GROUP
SensorAnalogGroupInput * mInput;
SensorAnalogGroupInput * mInput2;
#endif
int filterCount = 0;
HardwareSerial *ptLogSer;
HardwareSerial *ptInputSer1;
HardwareSerial *ptInputSer2;
HardwareSerial *ptEspSer;
Logger*  myLogger;
/**********************************************************************/
void setup() {

  /**********************************************************************/
  
  ptLogSer = &LOG_SERIAL;
  ptLogSer->begin(LOG_BAUDRATE); //USB
  myLogger= new Logger(ptLogSer,machineName,D);
  
  ptInputSer1 = &RS232_SERIAL2;
  ptInputSer1->begin(RS232_2_BAUDRATE);
  
  ptInputSer2 = &RS232_SERIAL3;
  ptInputSer2->begin(RS232_3_BAUDRATE);
  
  ptEspSer = &ESP_SERIAL;
  ptEspSer->begin(ESP_BAUDRATE);     //WIFI
  
  // serial
  // Serial.begin(LOG_BAUDRATE);
  // LOG_SERIAL.begin(LOG_BAUDRATE);     //USB
  //  RS232_SERIAL2.begin(RS232_2_BAUDRATE); //rs232
  // RS232_SERIAL3.begin(RS232_3_BAUDRATE); //rs232
  //  ptEspSer->begin(ESP_BAUDRATE);     //WIFI
  
  //ptLogSer->println(F("Setup start"));
  _LOG_PRINT(M,F("Setup start"),"");



  int pos = 0;
  boolean ledBuildIn = false;


  pinMode(ESP_RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  // initialisation des entree logiques
  //______________________________________

#ifdef OPTOGROUP
  ptOptoInputGroup = new  DigitalInput(ptEspSer, ptLogSer, sizeof(optoInput) / sizeof(int), optoInput, OPTOGROUP_NUMBER);
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
  const float refVoltage = 5000;      // in mV
  const float refVoltage2 = 20855;    // in mV !!!!!!!!!!!!!!!!!!!

  // const int refSource=DEFAULT;
  mInput = new SensorAnalogGroupInput(ptEspSer, ptLogSer, 'A', INT, pinNumber, scale, offset, refVoltage, DEFAULT);
  mInput2 = new SensorAnalogGroupInput(ptEspSer, ptLogSer, 'A', VOL, pinNumber2, scale2, offset2, refVoltage2, EXTERNAL);
#endif

  // reception des sorties logique
  //_______________________________
#ifdef LOGICAL_OUT
  mDigit = new DigitalOutput(8, relais);
#endif
  // barometre bmp180
  //__________

#ifdef BAROMETRE
  //ptLogSer->println(F("Setup start BAROMETRE"));
  
  _LOG_PRINT(M,F("setup"),F("Setup start BAROMETRE"));
  
  mySensor = new SensorBmp183(ptEspSer, ptLogSer);
  if (mySensor->getError()) {
    
    ptLogSer->print(F("erreur ouverture SensorBmp183 "));
    ptLogSer->print(mySensor->getErrorMsg());
    ptLogSer->println(F("*******"));
   
     _LOG_PRINT(M,F("erreur ouverture SensorBmp183 "),mySensor->getErrorMsg());   
    delete mySensor;
    mySensor = NULL;
    //ptLogSer->println(F("probleme ouverture SensorBmp183 ") );
     _LOG_PRINT(M,F("setup"),F("probleme ouverture SensorBmp183 "));
 
  }
  else {
    //ptLogSer->println(F("ouverture SensorBmp183 OK " ));
    
    _LOG_PRINT(M,F("setup"),F("ouverture SensorBmp183 OK " ));
  }
#endif
  delay(500);

  // girouette
  //__________

#ifdef GIROUETTE
  //ptLogSer->println(F("Setup start GIROUETTE"));
  _LOG_PRINT(M,F("setup"),F(" start GIROUETTE"));

  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN, X_OFFSET, Y_OFFSET, Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    
   // ptLogSer->println(F("ouverture SensorHMC5883 ") + mySensor2->getErrorMsg());
    _LOG_PRINT(M,F("ouverture SensorHMC5883 ") , mySensor2->getErrorMsg());
    
   
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    //ptLogSer->println(F("girouette ok"));
    //ptLogSer->println(mySensor2->getSensorDetail());
    
    _LOG_PRINT(M,F("girouette ok"),mySensor2->getSensorDetail());
    
  }

  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  
  //ptLogSer->println(F("fin setup anemometre "));
  
  _LOG_PRINT(M,F("setup"),F("fin setup anemometre "));
  
  delay(1000);
#endif


  timeCounter = 0;

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
  
  //ptLogSer->println(F("waiting esp start"));
  _LOG_PRINT(D,F("setup"),F("waiting esp start"));

  while (espReady == false) {
    if (ledBuildIn) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledBuildIn = false;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      ledBuildIn = true;
    }
    //ledBuildIn!=ledBuildIn;
    if (ptEspSer->available() > 0) {
      // get the new byte:
      char inChar = (char)ptEspSer->read();
      //ptLogSer->print(inChar);
      // add it to the msgin:
      msgWifiIn += inChar;
      if (inChar == '\n' && msgWifiIn.length() > 0) {
        ptLogSer->print(F(" From ESP :"));
        ptLogSer->print(msgWifiIn);
       // _LOG_PRINT(D,F("waiting esp start"),msgWifiIn);
        if (msgWifiIn.indexOf(F("Broadcasting")) >=0) {
          espReady = true;
          //ptLogSer->println(F(" Broadcasting received"));
          _LOG_PRINT(M,F("waiting esp start"),F(" Broadcasting received"));
        }
        msgWifiIn = "";
      }
    }
    //delay(100);


  }
  msgWifiIn = "";
  //ptLogSer->println(F("\n esp started"));
  _LOG_PRINT(M,F("setup"),F(" esp started"));
  
  digitalWrite(LED_BUILTIN, LOW);
#endif


  
  ptLogSer->print(F("setup end\r\n"));



#ifdef OPTOGROUP
//  ptLogSer->print(F("OPTOGROUP  TIME_INTERVAL_OPTO_INPUT :"));
//  ptLogSer->println(TIME_INTERVAL_OPTO_INPUT);
  _LOG_PRINT(M,F("PTOGROUP  TIME_INTERVAL_OPTO_INPUT "),TIME_INTERVAL_OPTO_INPUT);
  
#endif
#ifdef ANALOG_GROUP
  //ptLogSer->print(F("ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP :"));
  //ptLogSer->println(TIME_INTERVAL_ANALOG_GROUP);
  _LOG_PRINT(M,F("ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP "),TIME_INTERVAL_ANALOG_GROUP);

#endif
#ifdef RECEIVE_USB
  //ptLogSer->println(F("RECEIVE_USB"));
  _LOG_PRINT(M,F("setup"),F("RECEIVE_USB"));
#endif
#ifdef BAROMETRE
  //ptLogSer->println(F("BAROMETRE"));
  _LOG_PRINT(M,F("setup"),F("BAROMETRE"));
#endif
#ifdef GIROUETTE
  //ptLogSer->println(F("GIROUETTE"));
  _LOG_PRINT(M,F("setup"),F("GIROUETTE "));
#endif
#ifdef RS232_2
  //ptLogSer->print(F("RS232_2 "));
  //ptLogSer->println(RS232_2_BAUDRATE);
  _LOG_PRINT(M,F("RS232_2"),RS232_2_BAUDRATE);
#endif
#ifdef RS232_3
  //ptLogSer->print(F("RS232_3 "));
  //ptLogSer->println(RS232_3_BAUDRATE);
  _LOG_PRINT(M,F("RS232_3"),RS232_3_BAUDRATE);
#endif
#ifdef WIFI
  //ptLogSer->println(F("WIFI"));
  _LOG_PRINT(M,F("WIFI"),F("active "));
#endif
}


/**********************************************************************/

void loop() {

  /**********************************************************************/
  //ptLogSer->println(F("start loop"));
  //--------------------------------------------------------
  // reception
#ifdef WIFI
   if (ptEspSer->available() > 0) {
                                             /* ptLogSer->print(F("start ESP_SERIAL" ));
                                                ptLogSer->println(freeRam ());*/
    // get the new byte:
    char inChar = (char)ptEspSer->read();
                                            //ptLogSer->print(inChar);
    // add it to the msgin:
    msgWifiIn += inChar;
    if (inChar == '\n' && msgWifiIn.length() > 0) {
#ifdef LOGICAL_OUT
      // positionement des ports logiques
      mDigit->write(msgWifiIn);
#endif
      //ptLogSer->print(F("****** IN **********"));
      //ptLogSer->println(msgWifiIn);
      _LOG_PRINT(D,F("****** IN **********"),msgWifiIn);
      msgWifiIn = "";
    }
  }
#endif

  //______________________________________________________

#ifdef RECEIVE_USB
  trfSerial(&msgWifiOut, ptLogSer, ptEspSer);
#endif
  //___________________________________________________________

  // read from port  ser2:
  //_____________________

#ifdef RS232_2
  trfSerial(&inData2, ptInputSer1, ptEspSer);
#endif
  //___________________________________________________________

  // read from port  ser3:
  //_____________________

#ifdef RS232_3
  trfSerial(&inData3, ptInputSer2, ptEspSer);

#endif
  //_____________________________________________________
  //ptLogSer->println(F("end reading"));
  //ptLogSer->println(F("loop end receive"));

  if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {
  /*  ptLogSer->print(F("******** nbLoop :"));
    ptLogSer->println(timeCounter);*/
   /*  ptLogSer->print(F(" freeRam"));
     ptLogSer->println(freeRam ());*/
    timeCounter = 0;
    lastTime = millis();
    //------------------------------------------------------------

    //barometre
    //__________

#ifdef BAROMETRE
#ifdef DEBUD_BARO
   ptLogSer->print(F("start baro" ));
    ptLogSer->print(F("time: "));
    ptLogSer->println(millis());
    
    _LOG_PRINT(D,F(" start baro time "),millis());
    //ptLogSer->println(freeRam ());
   // ptEspSer->print(F("time: "));
   // ptEspSer->println(millis());
#endif
    if (mySensor != NULL) {
      ptLogSer->println(F("baro"));
      mySensor->getData();
      mySensor->toNmea();
    }

#endif
    //-------------------------------------------------------------
    // anemo girouette
    //________________
    //delay(100);
#ifdef GIROUETTE
    if (mySensor2 != NULL) {
      String msg;
      //ptLogSer->println(F("mySensor2 start"));
      
      _LOG_PRINT(D,F("anemo girouette"),F("mySensor2 start"));
      mySensor2->getData();
      msg = mySensor2->toString();
      //msg=mySensor2->toNmea();
      // ptLogSer->print(F("mySensor2 send :"));
      // #ifdef WIFI
      //     mUdp->send(msg);
      //  #endif
      ptLogSer->println(msg);
    }
    else {
      ptLogSer->println(F("mySensor2 = NULL"));
    }
#endif
    // lastTime=millis();
  }
  else {
    timeCounter++;
  }



  // lecture des entrees logiques
  //______________________________________

#ifdef OPTOGROUP
  //ptLogSer->println(F("OPTOGROUP Start"));
  if (mOptoInputInterval + TIME_INTERVAL_OPTO_INPUT < millis()) {
    /* ptLogSer->print(F("OPTOGROUP Start"));
     ptLogSer->println(freeRam ()); */

    ptOptoInputGroup->toNmea();
    mOptoInputInterval = millis();
  }
#endif
  // lecture des entrees analogiques
  //______________________________________
#ifdef ANALOG_GROUP
  if (nextTimeAnalogGroupInput + TIME_INTERVAL_ANALOG_GROUP < millis()) {
    ptLogSer->print(F("start ANALOG_GROUP" ));
    
    _LOG_PRINT(D,F(" ANALOG_GROUP" ),F("start" ));
    
   // ptLogSer->println(freeRam ());

    mInput->toNmea();
    mInput2->toNmea();
    ptLogSer->print(F("end ANALOG_GROUP" ));
   // ptLogSer->println(freeRam ());

    nextTimeAnalogGroupInput = millis();
  }
#endif

  //----------------------------------------------------------------
  // ptLogSer->println(F("loop end "));
}

/*****************************************************************************/

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
  String tst = F( "$PERMC,160023.65,A,40.07,S,77.21,W,5.5,185,090317,7.3,E*7A");
  return tst;
}


void trfSerial(String *inData, HardwareSerial *input, HardwareSerial *output) {
  
#ifdef DEBUG
#define DEBUG1
#undef DEBUG
#endif
  //ptLogSer->print(F(" trfSerial"));
  //ptLogSer->println(freeRam ());
  
  _LOG_PRINT(V,F(" trfSerial"),freeRam ());
  

  
  int i=0;
  while (input->available() > 0 && i<10) {
   
    //ptLogSer->print(F(" trfSerial"));
    //ptLogSer->println(i);
    
    _LOG_PRINT(D,F(" trfSerial"),i);
    
    char received = input->read();
    if (received != -1) {
      *inData += received;
#ifdef DEBUG
      ptLogSer->print(received);
#endif
      if (received == '\n') {
       // ptLogSer->println(*inData);
        output->println(*inData);
        
        _LOG_PRINT(D,F("*inData"),*inData);
        *inData = ""; // Clear received buffer
        break;
      }
    }
  }
#ifdef DEBUG1
#define DEBUG
#undef DEBUG1
#endif
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
