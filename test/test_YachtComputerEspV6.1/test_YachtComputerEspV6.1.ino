 
//#define RECEIVE_USB
//#define BAROMETRE
//#define GIROUETTE
//#define RS232_2
//#define RS232_3
//#define WIFI
//#define OPTOGROUP
#define ANALOG_GROUP
//#define LOGICAL_OUT
//#define DEBUG


#include"Logger.h"
#include <AsyncrSerial.h>

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
#define TIME_INTERVAL_BAROMETRE 30000
#define TIME_INTERVAL_GIROUETTE 30000
#define TIME_INTERVAL_OPTO_INPUT 10000
#define TIME_INTERVAL_ANALOG_GROUP 5000
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
SensorBmp183* sensorBMP183;
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
Logger *myLogger;

static long timeCounter;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

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
String* ptMgWifiIn = NULL;
String msgWifiOut = "";
int doneCount = 0;
unsigned long lastTime = 0;
boolean espReady = false;
//char wifiHeader[]= WIFI_HEADER;


#ifdef ANALOG_GROUP
SensorAnalogGroupInput * mInput;
//SensorAnalogGroupInput * mInput2;
#endif
int filterCount = 0;

HardwareSerial *ptLogSer;
HardwareSerial *ptInputSer1;
HardwareSerial *ptInputSer2;
HardwareSerial *ptEspSer;
AsyncrSerial*  asyncrLogSer;
AsyncrSerial*  asyncrInputSer1;
AsyncrSerial*  asyncrInputSer2;
AsyncrSerial*  asyncrEspSer;

/**********************************************************************/
void setup() {

  /**********************************************************************/
  
  ptLogSer = &LOG_SERIAL;
  ptLogSer->begin(LOG_BAUDRATE); //USB
  myLogger= new Logger(ptLogSer,machineName,E);
  
  ptInputSer1 = &RS232_SERIAL2;
  ptInputSer1->begin(RS232_2_BAUDRATE);
  
  ptInputSer2 = &RS232_SERIAL3;
  ptInputSer2->begin(RS232_3_BAUDRATE);
  
  ptEspSer = &ESP_SERIAL;
  ptEspSer->begin(ESP_BAUDRATE);     //WIFI

  asyncrLogSer=new AsyncrSerial(ptLogSer);
  asyncrInputSer1=new AsyncrSerial(ptInputSer1);
  asyncrInputSer2=new AsyncrSerial(ptInputSer2);
  asyncrEspSer=new AsyncrSerial(ptEspSer);
  // serial
  // Serial.begin(LOG_BAUDRATE);
  // LOG_SERIAL.begin(LOG_BAUDRATE);     //USB
  //  RS232_SERIAL2.begin(RS232_2_BAUDRATE); //rs232
  // RS232_SERIAL3.begin(RS232_3_BAUDRATE); //rs232
  //  ptEspSer->begin(ESP_BAUDRATE);     //WIFI
  
  _LOG_PRINT(M,F("Setup start"),"***********************************************************************************");



  int pos = 0;
  boolean ledBuildIn = false;


  pinMode(ESP_RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  // initialisation des entree logiques
  //______________________________________

#ifdef OPTOGROUP
  ptOptoInputGroup = new  DigitalInput(ptEspSer, myLogger, sizeof(optoInput) / sizeof(int), optoInput, OPTOGROUP_NUMBER);
#endif


  // initialisation des entree analogiques
  //______________________________________
#ifdef ANALOG_GROUP
/* const int pinNumber[] = {8, 9, 10, 11};
  const int pinNumber2[] = {12, 13, 14, 15};
  const float scale[] = {66, 66, 66, 66};
  const float scale2[] = {0.653, 1.2995, 1.2995, 1.2995};
  const int offset[] = {512, 512, 512, 512};
  const int offset2[] = {0, 0, 0, 0};
  const float refVoltage = 5000;      // in mV
  const float refVoltage2 = 20855;    // in mV !!!!!!!!!!!!!!!!!!!
*/
 const int pinNumber[] = {0, 1, 2, 3};
  const int pinNumber2[] = {12, 13, 14, 15};
  const float scale[] = {66, 66, 66, 66};
  const float scale2[] = {0.653, 1.2995, 1.2995, 1.2995};
  const int offset[] = {0, 0, 0, 0};
 // const int offset2[] = {0, 0, 0, 0};
  const float refVoltage[] ={5000,5000,5000,5000};      // in mV
  //const float refVoltage2[] = {20855,20855,20855,20855};    // in mV !!!!!!!!!!!!!!!!!!!

  // const int refSource=DEFAULT;
  mInput = new SensorAnalogGroupInput(ptEspSer,myLogger, 'A', INT, pinNumber, scale, offset, refVoltage, DEFAULT);
 // mInput2 = new SensorAnalogGroupInput(ptEspSer, ptLogSer, 'A', VOL, pinNumber2, scale2, offset2, refVoltage2, EXTERNAL);
#endif

  // reception des sorties logique
  //_______________________________
#ifdef LOGICAL_OUT
  mDigit = new DigitalOutput(8, relais);
#endif
  // barometre bmp180
  //__________

#ifdef BAROMETRE  
  _LOG_PRINT(M,F("setup"),F("Setup start BAROMETRE"));
  
  sensorBMP183 = new SensorBmp183(ptEspSer, myLogger);
  if (sensorBMP183->getError()) {
    _LOG_PRINT(M,F("erreur ouverture SensorBmp183 "),sensorBMP183->getErrorMsg());   
    delete sensorBMP183;
    sensorBMP183 = NULL;
  }
  else {
    _LOG_PRINT(M,F("setup"),F("ouverture SensorBmp183 OK " ));
  }
#endif
  delay(500);

  // girouette
  //__________

#ifdef GIROUETTE
  _LOG_PRINT(M,F("setup"),F(" start GIROUETTE"));

  mySensor2 = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN, X_OFFSET, Y_OFFSET, Z_OFFSET, 12345);
  if (mySensor2->getError()) {
    _LOG_PRINT(M,F("ouverture SensorHMC5883 ") , mySensor2->getErrorMsg()); 
    delete mySensor2;
    mySensor2 = NULL;
  }
  else {
    _LOG_PRINT(M,F("girouette ok"),mySensor2->getSensorDetail());
    
  }

  //anemometre
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  _LOG_PRINT(M,F("setup"),F("fin setup anemometre "));
  
  delay(1000);
#endif // GIROUETTE


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
  _LOG_PRINT(D,F("setup"),F("waiting esp start"));
  while (espReady == false) {
    if (ledBuildIn) {
      digitalWrite(LED_BUILTIN, HIGH);
      ledBuildIn = false;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      ledBuildIn = true;
    }
    ptMgWifiIn=asyncrEspSer->getMessage();
    if (ptMgWifiIn!=NULL){
      ptLogSer->print(*ptMgWifiIn);
      if (ptMgWifiIn->indexOf(F("Broadcasting")) >=0) {
        espReady = true;
        _LOG_PRINT(M,F("setup esp"),F(" esp started"));
      }
    }
  }
  
  digitalWrite(LED_BUILTIN, LOW);
#endif  //WIFI
/*-------------------------------------------------------------------------------------*/

#ifdef OPTOGROUP
  _LOG_PRINT(M,F("PTOGROUP  TIME_INTERVAL_OPTO_INPUT "),TIME_INTERVAL_OPTO_INPUT);
  
#endif
#ifdef ANALOG_GROUP
  _LOG_PRINT(M,"ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP ",TIME_INTERVAL_ANALOG_GROUP);

#endif
#ifdef RECEIVE_USB
  _LOG_PRINT(M,"setup","RECEIVE_USB");
#endif
#ifdef BAROMETRE
  _LOG_PRINT(M,"setup",F("BAROMETRE"));
#endif
#ifdef GIROUETTE
  _LOG_PRINT(M,F("setup"),F("GIROUETTE "));
#endif
#ifdef RS232_2
  _LOG_PRINT(M,F("RS232_2"),RS232_2_BAUDRATE);
#endif
#ifdef RS232_3
  _LOG_PRINT(M,F("RS232_3"),RS232_3_BAUDRATE);
#endif
#ifdef WIFI
  //ptLogSer->println(F("WIFI"));
  _LOG_PRINT(M,F("WIFI"),F("active "));
#endif
  _LOG_PRINT(M,F("setup"),F("********************END*********************** "));
}


/**********************************************************************/

void loop() {

  /**********************************************************************/
  //ptLogSer->println(F("start loop"));
  //--------------------------------------------------------
  // reception
 String * ptMsg;
#ifdef WIFI
  ptMsg=asyncrEspSer->getMessage();
  if (ptMsg!=NULL){
    
#ifdef LOGICAL_OUT
      // positionement des ports logiques
    mDigit->write(*ptMsg);
#endif //LOGICAL_OUT

    ptLogSer->println(*ptMsg);    
  }
#endif //WIFI

  //______________________________________________________

#ifdef RECEIVE_USB
    ptMsg=asyncrLogSer->getMessage();
    if (ptMsg!=NULL){
      ptEspSer->print(*ptMsg);
    }
#endif
  //___________________________________________________________

  // read from port  ser2:
  //_____________________

#ifdef RS232_2
    ptMsg=asyncrInputSer1->getMessage();
    if (ptMsg!=NULL){
      ptEspSer->print(*ptMsg);
      ptLogSer->print(*ptMsg);
    }
  //trfSerial(&inData2, ptInputSer1, ptEspSer);
#endif
  //___________________________________________________________

  // read from port  ser3:
  //_____________________

#ifdef RS232_3
  ptMsg=asyncrInputSer2->getMessage();
    if (ptMsg!=NULL){
      ptEspSer->print(*ptMsg);
      ptLogSer->print(*ptMsg);
    }
 // trfSerial(&inData3, ptInputSer2, ptEspSer);

#endif
  //_____________________________________________________
 
  if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {

    timeCounter = 0;
    lastTime = millis();
    //------------------------------------------------------------

    //barometre
    //__________

#ifdef BAROMETRE
  
    _LOG_PRINT(D,F(" start baro time "),millis());
    sensorBMP183->getData();
    _LOG_PRINT(D,"  baro  ","sensorBMP183->getData");
    sensorBMP183->toNmea();
    _LOG_PRINT(D,F("  baro  "),"sensorBMP183->toNmea");
 

#endif  //BAROMETRE
    //-------------------------------------------------------------
    // anemo girouette
    //________________
/* 
#ifdef GIROUETTE
    if (mySensor2 != NULL) {       
        char* cmsg;
        mySensor->getData();
       // ptLogOutputSer->println("mySensor->getData() done");
        cmsg=mySensor->toNmea();
       // ptLogOutputSer->println("mySensor->toNmea() done");
        _LOG_PRINT(V,"loop","mySensor->toNmea() done");
       // msg.toCharArray(cmsg,msg.length());
        sendMsg(cmsg);
        //ptLogOutputSer->println("msg sendet");
       // msg=mySensor->toString();
        //ptLogOutputSer->println(mySensor->toString());
         _LOG_PRINT(D,"msg sendet",mySensor->toString());

    }
    else {
     // ptLogSer->println(F("mySensor2 = NULL"));
      _LOG_PRINT(E,"Girouette",F("mySensor2 = NULL"));
    }
#endif //GIROUETTE
*/
    // lastTime=millis();
  }
  else {
    //timeCounter++;
  }



  // lecture des entrees logiques
  //______________________________________

#ifdef OPTOGROUP
  if (mOptoInputInterval + TIME_INTERVAL_OPTO_INPUT < millis()) {
    ptOptoInputGroup->toNmea();
    mOptoInputInterval = millis();
  }
#endif //OPTOGROUP

  // lecture des entrees analogiques
  //______________________________________
#ifdef ANALOG_GROUP
  if (nextTimeAnalogGroupInput + TIME_INTERVAL_ANALOG_GROUP < millis()) {
    _LOG_PRINT(M," ANALOG_GROUP" ,"start");
    Serial.println(mInput->toNmea());
    //mInput2->toNmea();
    _LOG_PRINT(M," ANALOG_GROUP" ,"end");
    nextTimeAnalogGroupInput = millis();
  }
#endif  //ANALOG_GROUP

  //----------------------------------------------------------------
 
}

/*****************************************************************************/

// interup routine pour Girouette
//_______________________________

#ifdef GIROUETTE
void anemoInterupt() {
  GanemoCount++;
}
#endif  //GIROUETTE

/**************************/



int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

