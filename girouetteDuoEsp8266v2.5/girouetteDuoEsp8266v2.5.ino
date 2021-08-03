#define QMC
#ifdef QMC
 // #define SENSOR SensorQMC5883Duo
  #include <SensorQMC5883.h>
   SensorQMC5883* sensorMag1;
  SensorQMC5883* sensorMag2;
#else
  //#define SENSOR SensorHMC5883
  #include <Adafruit_HMC5883_U.h>
  #include <SensorHMC5883.h>
  SensorHMC5883* sensorMag1;
  SensorHMC5883* sensorMag2;
#endif

#include "SensorGirouette.h"
#include"Logger.h"
#include <EEPROM.h>
//#include <EepromSrtream.h>
//#include <Decoder.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <NmeaMwv.h>
#include <SensorAnemometre.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>



#define LOG_LEVEL 0
#define BUFF_LENGTH 250
#define BROACAST_IP "192.168.4.255"
#define MY_IP 192,168,4,2
#define IPV 4
#define HOST_NAME "Bilbo_Main"
#define LOG_HEADER "***bilboGir***"
#define SERIAL_LOG Serial
//#define LOG_SERIAL Serial

//#define SERIAL_ARDUINO Serial


#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_GIROUETTE 1000
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 14
#define LED_PIN 2

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0

#undef ENTETE_COMMANDE
#define ENTETE_COMMANDE "#ANEMO#"
#undef LG_ENTETE_COMMANDE
#define LG_ENTETE_COMMANDE 7
#define SEP ","

/*******************************************************/

/********************/
/* global variables */
/********************/



char    machineName[] = "EpsGirouette";
Logger* myLogger;

//Wifi

const char* ssid = HOST_NAME;
const char* password = "marccypres";
WiFiUDP Udp;
unsigned int localUdpPort = 5416;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
IPAddress myIP(192, 168, 4, 2);
//IPAddress remoteIp;
IPAddress broadcastIp;
IPAddress gateway(192, 168, 4, 255);
IPAddress submet(255, 255, 255, 0);

boolean firstIp = false;
int mRemotePort = 0;
WiFiServer server(localUdpPort);

// girouette

/* Assign a unique ID to this sensor at the same time */
NmeaMwv* ptNmeaMwv;
SensorGirouette* sensorGirouette;
SensorAnemometre* sensorAnemometre;
float xOffset = 0;
float yOffset = 0;
float zOffset = 0;
unsigned long lastTime = 0;
/*
// valeurs Eeprom
EepromSrtream <EepromBuff>myEepromSrtream;
Decoder* ptDecoder = NULL;
EepromBuff* ptEepromBuff;
*/
// compteur girouette

HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
String inData = "";

/*************************************************************************************/


void setup() {

  ptLogOutputSer = &Serial;
  ptLogOutputSer->begin(115200);
  pinMode(16,OUTPUT);
  // LevelValue{M=0,E=1,W=2,D=3,V=4}

  myLogger = new Logger(ptLogOutputSer, machineName,W);
   _LOG_PRINT(M, machineName, "Start");

  //charger les param Eeprom
  //________________________
  /*
  ptDecoder = new Decoder(&myEepromSrtream, myLogger);
  ptEepromBuff = myEepromSrtream.readEeprom();
  //ptLogOutputSer->println(ptDecoder->toString());
  _LOG_PRINT(M, (char*)"mydecoder ", ptDecoder->toString());
  */
  // initiate I2C
  Wire.begin();

   //Start Wifi
   
  //WiFi.disconnect();
  #ifdef WIFI
    WiFi.begin(ssid, password);
    boolean ledPin = true;
    pinMode(LED_PIN, OUTPUT);
    while (WiFi.status() != WL_CONNECTED)
  
    {
      delay(500);
      ptLogOutputSer->print(".");
      digitalWrite(LED_PIN, ledPin);
      ledPin = !ledPin;
    }
    digitalWrite(LED_PIN, HIGH);
    ptLogOutputSer->println(" connected");
    _LOG_PRINT(M, (char*)"setup ", " connected");
    WiFi.mode(WIFI_STA);
    WiFi.config(myIP, gateway, submet);
  
    server.begin();
    _LOG_PRINT(M, (char*)"Connecting to ", ssid);
    Udp.begin(localUdpPort);
  
    // printWifiInfo();
  
    broadcastIp = GetBroadcastAddress(  WiFi.localIP().toString(), WiFi.subnetMask().toString());
  
    printWifiInfo();
#endif //WIFI

  // initialise interupt
  
  //pinMode(ANEMO_PIN, INPUT);
  //attachInterrupt(ANEMO_PIN, anemoInterupt, RISING);
  // attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);

  /* Initialise the sensor */
  
_LOG_PRINT(M, (char*)"setup"," Initialise the sensor");

#ifdef QMC
    sensorMag1= new SensorQMC5883(myLogger,0,0,0,0,0,0);
    sensorMag2= new SensorQMC5883(myLogger,0,0,0,0,0,0); 
#else
    sensorMag1= new SensorHMC5883(myLogger);
    sensorMag2= new SensorHMC5883(myLogger); 
#endif
    sensorGirouette = new SensorGirouette(myLogger,(SensorMagneto*)sensorMag1,(SensorMagneto*)sensorMag1);
    if (sensorGirouette==NULL)  {
        _LOG_PRINT(M, (char*)"ouverture SensorHMC5883  "," sensorGirouette==NULL");                        
    }
    else{
      if (sensorGirouette->getError()) {
  
        _LOG_PRINT(M, (char*)"Error ouverture SensorQMC5883  ", sensorGirouette->getErrorMsg());
        delete sensorGirouette;
        sensorGirouette = NULL;
      } 
      else {
  
        _LOG_PRINT(M, "girouette ok", sensorGirouette->getSensorDetail());
      }
    }
    sensorGirouette->init(QMC5883L_CONFIG_8GAUSS);
 // sensorAnemometre=new SensorAnemometre(myLogger);
    ptNmeaMwv =new NmeaMwv(myLogger);
    lastTime = 0;

    _LOG_PRINT(M, "main setup ***", " setup end ");
}
/*****************************************************************************************/
void loop() {
    _LOG_PRINT(V, "main loop ***", " Start loop ");
  // reception des paquets udp
  //___________________________
  
#ifdef WIFI
  inData = getPaket();
  if (inData.length() > 0) {
    _LOG_PRINT(D, " received :", inData);
    int n = inData.indexOf(ENTETE_COMMANDE);
    _LOG_PRINT(D, " inData.indexOf(ENTETE_COMMANDE) :", n);
    if (n >= 0) {
      n += LG_ENTETE_COMMANDE;
      _LOG_PRINT(D, " n:", n);
      _LOG_PRINT(D, "argument commande ", inData.substring(n));
      if (ptDecoder != NULL) {
        ptDecoder->decodeCommande(inData.substring(n));
        if (ptDecoder->getEepromBuff() != NULL) {
          sensorGirouette->xOffset = ptDecoder->getEepromBuff()->ini.x;
          sensorGirouette->yOffset = ptDecoder->getEepromBuff()->ini.y;
          sensorGirouette->zOffset = ptDecoder->getEepromBuff()->ini.z;

          sendMsg(sensorGirouette-> eepromParamtoString());
        }
        else {
          _LOG_PRINT(E, " ptDecoder->getEepromBuff()!=", "NULL");
        }
      }
      else {
        _LOG_PRINT(E, " mydecoder=", "NULL");
      }
    }
  }

  #endif WIFI



  // anemo girouette
  //________________



  if (sensorGirouette != NULL) {
    char *cmsg;
   // delay(100);
    if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {
      
      _LOG_PRINT(M, "loop", "***Start girouette***\n");
      _LOG_PRINT(V, "lastTime =", millis());
      
      lastTime = millis();
      
      sensorGirouette->getData();
      _LOG_PRINT(V, "loop", "sensorGirouette->getData() done");
     // cmsg = ptNmeaMwv->toNmea(sensorGirouette->getHeading(),sensorAnemometre->getSpeed());
      cmsg = ptNmeaMwv->toNmea(sensorGirouette->getHeading(),0);
      _LOG_PRINT(V, "loop", "sensorGirouette->toNmea() done");
      // *****************************************msg.toCharArray(cmsg,msg.length());
      sendMsg(cmsg);
      _LOG_PRINT(W, "msg sendet", sensorGirouette->toString());
     // ptLogOutputSer->print(sensorGirouette->toString());
    
    }
  }
  else {
    _LOG_PRINT(M, "girouette", "sensorGirouette = NULL");
  }
}

/***********************************************************************************************/
/*
void anemoInterupt() {
  GanemoCount++;
  if (state) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  state = !state;
}
*/
/************************************************************************************************/

void sendMsg(char* inData) {
  _LOG_PRINT(D, "sendMsg :", (char *)inData);
  #ifdef WIFI
  mRemotePort = 5416;
  Udp.beginPacket(broadcastIp, mRemotePort);
  //Udp.beginPacket(remoteIp, 5416);
  Udp.write(inData);
  Udp.endPacket();
  #endif
}

/***********************************************************************************************/

IPAddress GetBroadcastAddress(String ipAddress, String subnetMask) {
  //determines a broadcast address from an ip and subnet
  String ip[IPV] ;
  String mask[IPV];
  byte ipAdressBytes[IPV];
  byte subnetMaskBytes[IPV];

  split(ip, ipAddress, '.', IPV);
  split(mask, subnetMask, '.', IPV);
  for (int i = 0; i < IPV; i++) {
    ipAdressBytes[i] = ip[i].toInt();
    subnetMaskBytes[i] = mask[i].toInt();
  }

  uint8 broadcastAddress[IPV] ;
  for (int i = 0; i < IPV; i++) {
    broadcastAddress[i] = (ipAdressBytes[i] | (subnetMaskBytes[i] ^ 255));
  }
  return IPAddress(broadcastAddress[0], broadcastAddress[1], broadcastAddress[2], broadcastAddress[3]);
}
/************************************************************************************************************/
void split(String* output, String input, char sep, int len) {
  int startPos = 0;
  int endPos = 0;
  for (int i = 0; i < len && endPos >= 0; i++) {
    endPos = input.indexOf(sep, startPos);
    if (endPos >= 0) {
      output[i] = input.substring(startPos, endPos);
      startPos = endPos + 1;
    }
    else {
      output[i] = input.substring(startPos, input.length());
    }
  }

}
/***************************************************************************************************************/
String getPaket() {
  char incomingPacket[255];
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    _LOG_PRINT(D, "getPaket():Received ", packetSize);
    _LOG_PRINT(D, "byte from ", Udp.remoteIP().toString().c_str());
    _LOG_PRINT(D, " port ", Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
      _LOG_PRINT(D, "getPaket() msg:", incomingPacket);
      return String(incomingPacket);
    }
  }
  incomingPacket[0] = 0;
  return String(""); //incomingPacket;
}
void printWifiInfo() {
  _LOG_PRINT(V, "\t Local ip = ", WiFi.subnetMask().toString());
  _LOG_PRINT(V, "\t subnetMask = ", WiFi.subnetMask().toString());
  _LOG_PRINT(V, "\t broadcastIp = ", broadcastIp.toString());
  _LOG_PRINT(V, "\t port ", localUdpPort);

}
void printReceivedPaket(int packetSize ) {
  _LOG_PRINT(D, "Received ", packetSize);
  _LOG_PRINT(D, "byte from ", Udp.remoteIP().toString().c_str());
  _LOG_PRINT(D, " port ", Udp.remotePort());
}
