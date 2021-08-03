 

#define ANALOG_GROUP
#include <Wire.h>

#include "Nmea.h"
#include"Logger.h"
#include <AsyncrSerial.h>
#include <SensorAnalog.h>
#include "SensorAnalogGroupInput.h"
#define WIFI
// nmea message frame
#define NMEA_MWV_MSG "$PAMWV,000.0,R,000.00,N*     "

#define NMEA_VHW_MSG "$PAVHW,000.0,T,000.0,M,000.00,N,00.00,K*     "
/*
VHW
Water Speed and Heading
       1   2 3   4 5   6 7   8 9
       |   | |   | |   | |   | |
$--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh
1) Degress True
2) T = True
3) Degrees Magnetic
4) M = Magnetic
5) Knots (speed of vessel relative to the water)
6) N = Knots
7) Kilometers (speed of vessel relative to the water)
8) K = Kilometres
9) Checksum
*/
//#include <Sensor.h>

#include <Adafruit_Sensor.h>

#include "SensorAnalogGroupInput.h"

#define WIFI_HEADER F("***ESP")
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 30000
#define TIME_INTERVAL_GIROUETTE 30000
#define TIME_INTERVAL_OPTO_INPUT 10000
#define TIME_INTERVAL_ANALOG_GROUP 500
#define TIME_INTERVAL3 150
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

#define FILTERCOUNT 30

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416
#define ESP_RESET_PIN  7

// serial ports
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200


#define RS232_3_BAUDRATE 115200
#define RS232_2_BAUDRATE 38400
#define RS232_SERIAL3 Serial3

#define ESP_SERIAL Serial1
#define RS232_SERIAL2 Serial2
#define LOG_SERIAL Serial


#define OPTOGROUP_NUMBER 1


SensorAnalog* mAnalogSensor;


//************************************************

//Variables globales

//************************************************
long unsigned sendCound=0;
Nmea* nmeaMWv;
static char nmeaMWvmsg[]=NMEA_MWV_MSG;
 float nmeaMWvparamList[2];
 Nmea* nmeaHDT;
 static char nmeaHDTmsg[]=NMEA_VHW_MSG;
 float nmeaHDTparamList[2];
/*
VHW
Water Speed and Heading
       1   2 3   4 5   6 7   8 9
       |   | |   | |   | |   | |
$--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh
1) Degress True
2) T = True
3) Degrees Magnetic
4) M = Magnetic
5) Knots (speed of vessel relative to the water)
6) N = Knots
7) Kilometers (speed of vessel relative to the water)
8) K = Kilometres
9) Checksum
*/
char    machineName[]="YcMain";
Logger *myLogger;

static long timeCounter;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor



unsigned long nextOptoInput = 0;
unsigned long nextTimeBaro = 0;
unsigned long nextTimeAnemo = 0;
unsigned long nextTimeAnalogGroupInput = 0;
unsigned long lastTimeRs232;

// compteur girouette


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
  myLogger= new Logger(ptLogSer,machineName,M);
  
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
 
  
  _LOG_PRINT(M,F("Setup start"),"***********************************************************************************");



  //int pos = 0;
  boolean ledBuildIn = false;


  pinMode(ESP_RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);


  // initialisation des entree analogiques
  //______________________________________
#ifdef ANALOG_GROUP

 const int pinNumber[] = {0, 1, 2, 3};
  const int pinNumber2[] = {12, 13, 14, 15};
  const float scale[] = {13.88, 66, 13.88, 66};
  const float scale2[] = {0.653, 1.2995, 1.2995, 1.2995};
  const int offset[] = {0, 0, 0, 0};
  const float refVoltage[] ={5000,5000,5000,5000};      // in mV
  mInput = new SensorAnalogGroupInput(ptEspSer,myLogger, 'A', INT, pinNumber, scale, offset, refVoltage, DEFAULT);
#endif

 // Nmea messages

//"$PAMWV,000.0,R,000.00,N*     "
// 0123456789012345678901234567890

 signed char paramSize[2]={5,6};
 unsigned char paramPrec[2]={1,2};
 int paramPos[2]={7,15};
 nmeaMWv=new Nmea(myLogger,nmeaMWvmsg,2, nmeaMWvparamList,paramSize,paramPrec,paramPos);

 //"$PAVHW,000.0,T,000.0,M,00.00,N,00.00,K*     "
//  012345678901234567890123456789012345678901234
/*
VHW
Water Speed and Heading
       1   2 3   4 5   6 7   8 9
       |   | |   | |   | |   | |
$--VHW,x.x,T,x.x,M,x.x,N,x.x,K*hh
1) Degress True
2) T = True
3) Degrees Magnetic
4) M = Magnetic
5) Knots (speed of vessel relative to the water)
6) N = Knots
7) Kilometers (speed of vessel relative to the water)
8) K = Kilometres
9) Checksum
*/
 signed char paramHDTSize[2]={5,6};
 unsigned char paramHDTPrec[2]={1,2};
 int paramHDTPos[2]={7,23};
 nmeaHDT=new Nmea(myLogger,nmeaHDTmsg,2, nmeaHDTparamList,paramHDTSize,paramHDTPrec,paramHDTPos);


  delay(500);

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
  _LOG_PRINT(M,F("setup"),F("waiting esp start"));
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

#ifdef ANALOG_GROUP
  _LOG_PRINT(M,"ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP ",TIME_INTERVAL_ANALOG_GROUP);

#endif
#ifdef RECEIVE_USB
  _LOG_PRINT(M,"setup","RECEIVE_USB");
#endif
#ifdef WIFI
  //ptLogSer->println(F("WIFI"));
  _LOG_PRINT(M,F("WIFI"),F("active "));
#endif
  _LOG_PRINT(M,F("setup"),F("********************SETUP END*********************** "));
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
  //______________________________________________________


  // lecture des entrees analogiques
  //______________________________________

  if (nextTimeAnalogGroupInput + TIME_INTERVAL_ANALOG_GROUP < millis()) {
    //_LOG_PRINT(M," ANALOG_GROUP" ,"start");
    //Serial.println(mInput->toNmea());
    //mInput2->toNmea();
    nmeaMWvparamList[0]=mInput->getEntryValue(0);
    nmeaMWvparamList[1]=mInput->getEntryValue(1);
   // Serial.println(nmeaMWv->ToNmea());
    ptEspSer->print(sendCound++);
    ptEspSer->print("\t");
    ptEspSer->println(nmeaMWv->ToNmea());
    delay(500);
    nmeaHDTparamList[0]=mInput->getEntryValue(2);
    nmeaHDTparamList[1]=mInput->getEntryValue(3);
   // Serial.println(nmeaHDT->ToNmea());
    ptEspSer->print(sendCound++);
    ptEspSer->print("\t");
    ptEspSer->println(nmeaHDT->ToNmea());
    
    //_LOG_PRINT(M," ANALOG_GROUP" ,"end");

    /*
     HDT
Heading – True
       1   2 3
       |   | |
$--HDT,x.x,T*hh
1) Heading Degrees, true
2) T = True
3) Checksum 
     */
    nextTimeAnalogGroupInput = millis();
  }
  

  //----------------------------------------------------------------
 
}




