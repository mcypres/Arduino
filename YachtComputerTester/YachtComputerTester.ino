 
#define RECEIVE_USB
//#define WIFI
#define ANALOG_GROUP
//#define DEBUG

#include <TestNmea.h>
#include"Logger.h"
#include <AsyncrSerial.h>


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

#define NMEA_MWV_MSG "$PAMWV,000.00,R,000.00,N,A*    "
//********************0123456789012345678901234567890
#define NMEA_HDM_MSG "$PAHDM,000.00,M*    "
//********************01234567890123456789

#define WIFI_HEADER F("***ESP")
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 30000
#define TIME_INTERVAL_GIROUETTE 3000
#define TIME_INTERVAL_OPTO_INPUT 10000
#define TIME_INTERVAL_ANALOG_GROUP 5000
#define TIME_INTERVAL3 150
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

#define FILTERCOUNT 30

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0
#define SPEED_COEF 1

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416
#define ESP_RESET_PIN  7

// serial ports
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200

//#define RS232_2_BAUDRATE 4800
//#define RS232_3_BAUDRATE 4800
//#define RS232_3_BAUDRATE 115200
#define RS232_2_BAUDRATE 38400
#define RS232_3_BAUDRATE 38400
//#define RS232_2_BAUDRATE 19200
//#define RS232_3_BAUDRATE 19200

#define RS232_SERIAL3 Serial3

#define ESP_SERIAL Serial1
#define RS232_SERIAL2 Serial2
#define LOG_SERIAL Serial


#define OPTOGROUP_NUMBER 1




//************************************************

//Variables globales

//************************************************
TestNmea* Compas;
TestNmea* girouette;
TestNmea* xxx;
char msgMWV[]=NMEA_MWV_MSG;
char msgHDM[]=NMEA_HDM_MSG;

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


String inData2;// input buffer rs232 port2
String inData3;// input buffer rs232 port3
String* ptMgWifiIn = NULL;
String msgWifiOut = "";
int doneCount = 0;
unsigned long lastTime = 0;
boolean espReady = false;


#ifdef ANALOG_GROUP
//SensorAnalogGroupInput * mInput;
//SensorAnalogGroupInput * mInput2;
#endif
int filterCount = 0;

HardwareSerial *ptUSB;
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
    
    ptUSB = &LOG_SERIAL;
    ptUSB->begin(LOG_BAUDRATE); //USB
    myLogger= new Logger(ptUSB,machineName,M);
    
    ptInputSer1 = &RS232_SERIAL2;
    ptInputSer1->begin(RS232_2_BAUDRATE);
    
    ptInputSer2 = &RS232_SERIAL3;
    ptInputSer2->begin(RS232_3_BAUDRATE);
    
    ptEspSer = &ESP_SERIAL;
    ptEspSer->begin(ESP_BAUDRATE);     //WIFI
  
    asyncrLogSer=new AsyncrSerial(ptUSB);
    asyncrInputSer1=new AsyncrSerial(ptInputSer1);
    asyncrInputSer2=new AsyncrSerial(ptInputSer2);
    asyncrEspSer=new AsyncrSerial(ptEspSer);
   
    
    _LOG_PRINT(M,F("Setup start"),"***********************************************************************************");
  
  
  
    int pos = 0;
    boolean ledBuildIn = false;
  
  
    pinMode(ESP_RESET_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
  
  
  
  
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
      }//if (ledBuildIn)
      ptMgWifiIn=asyncrEspSer->getMessage();
      if (ptMgWifiIn!=NULL){
        ptUSB->print(*ptMgWifiIn);
        if (ptMgWifiIn->indexOf(F("Broadcasting")) >=0) {
          espReady = true;
          _LOG_PRINT(M,F("setup esp"),F(" esp started"));
        }//if (ptMgWifiIn->...
      }//if (ptMgWifiIn!=NULL)
    }//if (ptMgWifiIn!=NULL)
  
  digitalWrite(LED_BUILTIN, LOW);
#endif  //WIFI


  _LOG_PRINT(M,"msgHDM",msgMWV);
    static const int portListHDM[]={7};
    static const int posListHDM[]={7};
    static const float coefListHDM[]={0.3519};
    Compas= new TestNmea(ptEspSer, myLogger,msgHDM,portListHDM,posListHDM,coefListHDM,1);
  
    
    //char msgMWV[]=NMEA_MWV_MSG;
    _LOG_PRINT(M,"msgMWV",msgMWV);
    static const int portListMWV[]={5,6};
    static const int posListMWV[]={7,16};
    static  float coefListMWV[]={0.3519,1};//0.9775};
    girouette= new TestNmea(ptEspSer, myLogger,msgMWV,portListMWV,posListMWV,coefListMWV,2);
  /*-------------------------------------------------------------------------------------*/
  
  
  /**********************************************************************/
}//setup
void loop() {

    /**********************************************************************/
    //ptUSB->println(F("start loop"));
    //--------------------------------------------------------
    // reception
   
   String * ptMsg = new String("No message");
   
  
  
    //______________________________________________________
  
  #ifdef RECEIVE_USB
      
      if (ptMsg!=NULL){
        
        ptMsg=asyncrLogSer->getMessage();
        ptEspSer->print(*ptMsg);
      }
      else{
        _LOG_PRINT(V,F("ptMsg")," is NULL");
      }//if (ptMsg
  #endif
    //___________________________________________________________
  
    ptUSB->println(String(girouette->toNmea()));
    delay(200);
    ptUSB->println(String(Compas->toNmea()));
  

}


