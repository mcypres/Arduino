#include <TestNmea.h>
#include"Logger.h"
#include <AsyncrSerial.h>

#define NMEA_MWV_MSG "$PAMWV,000.00 ,R,000.00 ,N,A*    "
//********************0123456789012345678901234567890
#define NMEA_HDM_MSG "$PAHDM,000.00 ,M*    "
//********************01234567890123456789

#define NMEA_RMC_MSG "$PARMC246060,0000.00,S,yyyy.yy,E,00.0,000.0,123119,0.0,w*    "
//********************01234567890123456789012345678901234567890123456789012345678901
#define WIFI

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416
#define ESP_RESET_PIN  7

// serial ports
#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200
#define LOG_SERIAL Serial
#define ESP_SERIAL Serial1

// WIFI

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   5416
#define ESP_RESET_PIN  7
//************************************************

//Variables globales

//************************************************
TestNmea* Compas;
TestNmea* girouette;
TestNmea* xxx;
char msgMWV[]=NMEA_MWV_MSG;
char msgHDM[]=NMEA_HDM_MSG;
char msgRMC[]=NMEA_RMC_MSG;

int ledPin = 13;      // select the pin for the LED

char    machineName[]="YcMain";
Logger *myLogger;

HardwareSerial *ptUSB;
AsyncrSerial*  asyncrLogSer;

HardwareSerial *ptEspSer;
AsyncrSerial*  asyncrEspSer;
boolean espReady = false;

String* ptMgWifiIn = NULL;
String msgWifiOut = "";

void setup() {
 
   ptUSB = &LOG_SERIAL;
   ptUSB->begin(LOG_BAUDRATE); //USB
   myLogger= new Logger(ptUSB,machineName,M);

   ptEspSer = &ESP_SERIAL;
   ptEspSer->begin(ESP_BAUDRATE);     //WIFI

   asyncrLogSer=new AsyncrSerial(ptUSB);
   asyncrEspSer=new AsyncrSerial(ptEspSer);
   
   pinMode(ESP_RESET_PIN, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
 
  //reset esp8266
  //____________
#ifdef WIFI

  boolean ledBuildIn = false;
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
      ptUSB->print(*ptMgWifiIn);
      if (ptMgWifiIn->indexOf(F("Broadcasting")) >=0) {
        espReady = true;
        _LOG_PRINT(M,F("setup esp"),F(" esp started"));
      }
    }
  }
  
  digitalWrite(LED_BUILTIN, LOW);
#endif  //WIFI

  _LOG_PRINT(M,"msgHDM",msgHDM);
  _LOG_PRINT(M,"msgHDM","0123456789012345678901234567890");
    static const int portListHDM[]={7};
    static const int posListHDM[]={7};
    static const float coefListHDM[]={0.3519};
    Compas= new TestNmea(ptEspSer, myLogger,msgHDM,portListHDM,posListHDM,coefListHDM,1);
  
    
    //char msgMWV[]=NMEA_MWV_MSG;
    _LOG_PRINT(M,"msgMWV",msgMWV);
    _LOG_PRINT(M,"msgHDM","0123456789012345678901234567890");
    static const int portListMWV[]={5,6};
    static const int posListMWV[]={7,17};
    static  float coefListMWV[]={0.3519,1};//0.9775};
    girouette= new TestNmea(ptEspSer, myLogger,msgMWV,portListMWV,posListMWV,coefListMWV,2);
   static const int portListRMC[]={4,6};
    static const int posListRMC[]={7,17};
    static  float coefListRMC[]={0.3519,1};//0.9775};
 
     gps= new TestNmea(ptEspSer, myLogger,nmsgRMC,portListRMC,posListRMC,coefListRMC,2);
    
  /*-------------------------------------------------------------------------------------*/
  
  
}

void loop() {
   Serial1.println(String(girouette->toNmea()));
    delay(10);
   Serial1.println(String(Compas->toNmea()));    
    delay(10);
}
