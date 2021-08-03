//#define  DEBUG

#include"Logger.h"
#include <EEPROM.h>
#include <EepromSrtream.h>
#include <Decoder.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SensorHMC5883.h>


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
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
SensorHMC5883* mySensor;

float xOffset = 0;
float yOffset = 0;
float zOffset = 0;
unsigned long lastTime = 0;

// valeurs Eeprom
EepromSrtream <EepromBuff>myEepromSrtream;
Decoder* ptDecoder = NULL;
EepromBuff* ptEepromBuff;
// compteur girouette

volatile unsigned long GanemoCount;
boolean state = true;

HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
String inData = "";

/*************************************************************************************/


void setup() {

  ptLogOutputSer = &Serial;
  ptLogOutputSer->begin(115200);
  // LevelValue{M=0,E=1,W=2,D=3,V=4}

  myLogger = new Logger(ptLogOutputSer, machineName,D);
  //ptLogOutputSer->print(LOG_HEADER);
  //ptLogOutputSer->println("Start");
  _LOG_PRINT(M, machineName, "Start");

  //charger les param Eeprom
  //________________________
  ptDecoder = new Decoder(&myEepromSrtream, myLogger);
 // ptEepromBuff = myEepromSrtream.readEeprom();
  //ptLogOutputSer->println(ptDecoder->toString());
  _LOG_PRINT(M, (char*)"mydecoder ", ptDecoder->toString());
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  //Start Wifi
  //WiFi.disconnect();
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

  // initialise interupt
  pinMode(ANEMO_PIN, INPUT);
  attachInterrupt(ANEMO_PIN, anemoInterupt, RISING);
  /* Initialise the sensor */

 /* mySensor = new SensorHMC5883( ptLogOutputSer,
                                myLogger,
                                DECLINAISON_DEG,
                                DECLINAISON_MIN,
                                ptEepromBuff->ini.x,
                                ptEepromBuff->ini.y,
                                ptEepromBuff->ini.z,
                                ptEepromBuff->ini.speedCoef,
                                12345);*/
  mySensor = new SensorHMC5883( ptLogOutputSer,
                                myLogger,
                                DECLINAISON_DEG,
                                DECLINAISON_MIN,
                                0,
                                0,
                                0,
                                1,
                                12345);
  if (mySensor==NULL)  {
        _LOG_PRINT(M, (char*)"ouverture SensorHMC5883  "," mySensor==NULL");                        
  }
  else{
    if (mySensor->getError()) {
  
      _LOG_PRINT(M, (char*)"ouverture SensorHMC5883  ", mySensor->getErrorMsg());
      delete mySensor;
      mySensor = NULL;
    } 
    else {
  
      _LOG_PRINT(M, "girouette ok", mySensor->getSensorDetail());
    }
  }

  // charger les param Eeprom
  // ptLogOutputSer->println("charger les param Eeprom :readEeprom()");
  _LOG_PRINT(M, "main setup ***", " charger les param Eeprom :readEeprom() ");

  ptEepromBuff = myEepromSrtream.readEeprom();
  _LOG_PRINT(M, "ptDecoder->toString ***", ptDecoder->toString());
  //ptLogOutputSer->println(ptDecoder->toString());
  sendMsg(mySensor-> eepromParamtoString());
  lastTime = 0;

  //ptLogOutputSer->println("*********************setup end***************************");
  _LOG_PRINT(M, "main setup ***", " setup end ");
}
/*****************************************************************************************/
void loop() {
  //#ifdef DEBUG
  // ptLogOutputSer->println("***Start loop***");
  _LOG_PRINT(V, "main loop ***", " Start loop ");
  //#endif
  // reception des paquets udp
  //___________________________

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
          mySensor->xOffset = ptDecoder->getEepromBuff()->ini.x;
          mySensor->yOffset = ptDecoder->getEepromBuff()->ini.y;
          mySensor->zOffset = ptDecoder->getEepromBuff()->ini.z;

          sendMsg(mySensor-> eepromParamtoString());
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





  // anemo girouette
  //________________



  if (mySensor != NULL) {
    char *cmsg;
    delay(100);
    if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {
      _LOG_PRINT(V, "loop", "***Start girouette***\n");
      _LOG_PRINT(V, "lastTime =", millis());
      lastTime = millis();
      mySensor->getData();
      _LOG_PRINT(V, "loop", "mySensor->getData() done");
      cmsg = mySensor->toNmea();
      _LOG_PRINT(V, "loop", "mySensor->toNmea() done");
      // msg.toCharArray(cmsg,msg.length());
      sendMsg(cmsg);
      _LOG_PRINT(V, "msg sendet", mySensor->toString());
    }
  }
  else {
    _LOG_PRINT(M, "girouette", "mySensor = NULL");
  }
}

/***********************************************************************************************/

void anemoInterupt() {
  GanemoCount++;
  if (state) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  state = !state;
}

/************************************************************************************************/

void sendMsg(char* inData) {
  _LOG_PRINT(M, "sendMsg :", (char *)inData);
  mRemotePort = 5416;
  Udp.beginPacket(broadcastIp, mRemotePort);
  //Udp.beginPacket(remoteIp, 5416);
  Udp.write(inData);
  Udp.endPacket();
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
