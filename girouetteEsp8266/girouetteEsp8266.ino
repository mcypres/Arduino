#include <EEPROM.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SensorHMC5883.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

# define BUFF_LENGTH 250
#define BROACAST_IP "192.168.4.255"
#define MY_IP 192,168,4,2
#define IPV 4
#define HOST_NAME "Bilbo_Main"
#define LOG_HEADER "***bilbogir***"
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

/* global variables */

//Wifi

const char* ssid = HOST_NAME;
const char* password = "marccypres";
WiFiUDP Udp;
unsigned int localUdpPort = 5416;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
//IPAddress myIP(192,168,4,2);
//IPAddress remoteIp;
IPAddress broadcastIp;
//IPAddress gateway(192,168,4,10);
IPAddress submet(255,255,255,0);

boolean firstIp=false;
int mRemotePort=0;
WiFiServer server(localUdpPort);

// girouette

/* Assign a unique ID to this sensor at the same time */
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
SensorHMC5883* mySensor;

float xOffset= 0;
float yOffset= 0;
float zOffset= 0;

// compteur girouette

volatile unsigned long GanemoCount;
unsigned long lastTime=0;
boolean state=true;
HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;

/*************************************************************************************/


void setup()
{
 ptLogOutputSer=&Serial;
  ptLogOutputSer->begin(115200);
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->println("Start");
 
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  //Start Wifi
  //WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    ptLogOutputSer->print(".");
  }
  Wifi.mode(WIFI_STA);
  ptLogOutputSer->println(" connected");
  server.begin();
  //WiFi.config(myIP,gateway,submet); 
 
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->printf("Connecting to %s ", ssid);
 

  Udp.begin(localUdpPort);
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->printf("    Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
 
  broadcastIp=GetBroadcastAddress(  WiFi.localIP().toString(), WiFi.subnetMask().toString());
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->print( "    Local ip = ");
  ptLogOutputSer->println(WiFi.localIP().toString());
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->print( "    subnetMask = "),
  ptLogOutputSer->println(WiFi.subnetMask().toString());
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->print( "    broadcastIp = ");
  ptLogOutputSer->println(broadcastIp.toString());
  ptLogOutputSer->print(LOG_HEADER);
  ptLogOutputSer->printf("    Broadcasting at IP %s, UDP port %d\n", broadcastIp.toString().c_str(), localUdpPort);
  // initialise interupt
  pinMode(ANEMO_PIN, INPUT);
  attachInterrupt(ANEMO_PIN, anemoInterupt, RISING);
    /* Initialise the sensor */

  mySensor = new SensorHMC5883(ptLogOutputSer,ptLogOutputSer,DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor->getError()) {
   ptLogOutputSer->println("ouverture SensorHMC5883 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
  }
  else {
    ptLogOutputSer->println("girouette ok");
    ptLogOutputSer->println(mySensor->getSensorDetail());
  }
}

void loop()
{
  /*
  int packetSize = Udp.parsePacket();
  if (packetSize){
    // receive incoming UDP packets
   // ptLogOutputSer->printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0){
      incomingPacket[len] = 0;
    }*/
    
    ptLogOutputSer->print(LOG_HEADER);
    ptLogOutputSer->print("    received :");
    ptLogOutputSer->printf("%s\n", getPaket());
    
    //remoteIp=Udp.remoteIP();
    //firstIp=true;
   /* Udp.beginPacket(Udp.remoteIP(), 5416);
    Udp.write(replyPacekt);
    Udp.endPacket();*/
  //}
   // anemo girouette
  delay(100);
  if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
    lastTime=millis(); 
      if (mySensor != NULL) {
       // String msg;
        
        char* cmsg;
        mySensor->getData();
        cmsg=mySensor->toNmea();
       // msg.toCharArray(cmsg,msg.length());
        sendMsg(cmsg);
#ifdef DEBUG       
       // msg=mySensor->toString();
        ptLogOutputSer->println(mySensor->toString());
#endif
      }
      else{
        ptLogOutputSer->println("mySensor = NULL");
      }
    }
  
  }
  
void anemoInterupt() {
  GanemoCount++;
  if (state){
   digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  state =!state;
}

 void sendMsg(char* inData) {
    SERIAL_LOG.print(LOG_HEADER);
    SERIAL_LOG.print("    sendMsg :");
    SERIAL_LOG.println((char *)inData);
    mRemotePort=5416;
   //ptLogOutputSer->printf("send to %s, port %d\n",  broadcastIp.toString().c_str(), mRemotePort);
 
   
    Udp.beginPacket(broadcastIp, mRemotePort);
    //Udp.beginPacket(remoteIp, 5416);
    Udp.write(inData);
    Udp.endPacket();

  }
  
IPAddress GetBroadcastAddress(String ipAddress, String subnetMask) {
    //determines a broadcast address from an ip and subnet
    String ip[IPV] ;
    String mask[IPV];
    byte ipAdressBytes[IPV];
    byte subnetMaskBytes[IPV];
    
    split(ip,ipAddress,'.',IPV);
    split(mask,subnetMask,'.',IPV);
    for(int i=0;i<IPV;i++){
      ipAdressBytes[i]=ip[i].toInt();
      subnetMaskBytes[i]=mask[i].toInt();
    }

    uint8 broadcastAddress[IPV] ;
    for (int i = 0; i <IPV; i++) {
        broadcastAddress[i]= (ipAdressBytes[i] | (subnetMaskBytes[i] ^ 255));
     }
   return IPAddress(broadcastAddress[0],broadcastAddress[1],broadcastAddress[2],broadcastAddress[3]);
   }
 void split(String* output,String input,char sep,int len){
    int startPos=0;
    int endPos=0;
    for (int i=0;i<len && endPos>=0;i++){
      endPos =input.indexOf(sep,startPos);
      if(endPos>=0){
        output[i]=input.substring(startPos,endPos);
         startPos=endPos+1;
      }
      else{
        output[i]=input.substring(startPos,input.length());
      }
    }
  }
  char* getPaket(){
    int packetSize = Udp.parsePacket();
    if (packetSize){
      // receive incoming UDP packets
     // ptLogOutputSer->printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0){
        incomingPacket[len] = 0;
        return incomingPacket;
      }
    }
    return NULL;
  }

