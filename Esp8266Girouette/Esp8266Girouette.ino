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

#define TIME_INTERVAL_GIROUETTE 3000
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 14
#define LED_PIN 2

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0


const char* ssid = HOST_NAME;
const char* password = "marccypres";
WiFiUDP Udp;
unsigned int localUdpPort = 5416;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
//IPAdd
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_GIress myIP(192,168,4,2);
//IPAddress remoteIp;
IPAddress broadcastIp;
//IPAddress gateway(192,168,4,10);
IPAddress submet(255,255,255,0);

boolean firstIp=false;
int mRemotePort=0;
WiFiServer server(localUdpPort);

/* Assign a unique ID to this sensor at the same time */
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
SensorHMC5883* mySensor;
//int sensorValue = 0;  // variable to store the value coming from the sensor
//float cap=0;
//HardwareSerial *ptLogSer;

// compteur girouette

volatile unsigned long GanemoCount;
unsigned long lastTime=0;
boolean state=true;

/************************************************************************************/

void setup()
{
 // ptLogSer = &LOG_SERIAL;
  Serial.begin(115200);
  Serial.print(LOG_HEADER);
  Serial.println("Start");
 
  attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
  //Start Wifi
  WiFi.disconnect();
  Serial.print("try to connect to :");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  //server.begin();
  //WiFi.config(myIP,gateway,submet); 
  Serial.print(LOG_HEADER);
  Serial.printf("  Connected to %s local IP : %s  subnet mask : %s \n", ssid, WiFi.localIP().toString().c_str(),WiFi.subnetMask().toString().c_str());

  Udp.begin(localUdpPort);
  Serial.print(LOG_HEADER);
  Serial.printf("    Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
 
  broadcastIp=GetBroadcastAddress(  WiFi.localIP().toString(), WiFi.subnetMask().toString());
  Serial.print(LOG_HEADER);
  Serial.print( "    Local ip = ");
  Serial.println(WiFi.localIP().toString());
  Serial.print(LOG_HEADER);
  Serial.print( "    subnetMask = "),
  Serial.println(WiFi.subnetMask().toString());
  Serial.print(LOG_HEADER);
  Serial.print( "    broadcastIp = ");
  Serial.println(broadcastIp.toString());
  Serial.print(LOG_HEADER);
  Serial.printf("    Broadcasting at IP %s, UDP port %d\n", broadcastIp.toString().c_str(), localUdpPort);
  // initialise interupt
  pinMode(ANEMO_PIN, INPUT);
  attachInterrupt(ANEMO_PIN, anemoInterupt, RISING);
    /* Initialise the sensor */

  mySensor = new SensorHMC5883(&SERIAL_LOG,&SERIAL_LOG,DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor->getError()) {
   Serial.println("ouverture SensorHMC5883 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
  }
  else {
    Serial.println("girouette ok");
    Serial.println(mySensor->getSensorDetail());
  }
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }/*
    Serial.printf("%s\n", incomingPacket);
    Serial.print(LOG_HEADER);
    Serial.print("    received :");
    Serial.printf("%s\n", incomingPacket);*/
    //remoteIp=Udp.remoteIP();
    //firstIp=true;
   /* Udp.beginPacket(Udp.remoteIP(), 5416);
    Udp.write(replyPacekt);
    Udp.endPacket();*/
  }
   // anemo girouette
  //delay(100);
  if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
    lastTime=millis(); 
    if (mySensor != NULL) {
        char * cmsg;
        mySensor->getData();
        cmsg=mySensor->toNmea();
        sendMsg(cmsg);
#ifdef DEBUG 
        String msg;
        msg=mySensor->toString();      
        Serial.print(LOG_HEADER);
        Serial.print("    msg :");
        Serial.println(msg);
//#endif
        
        //msg.toCharArray(cmsg,msg.length()+1);
//#ifdef DEBUG       
        Serial.print(LOG_HEADER);
        Serial.print("    cmsg :");
        Serial.println(cmsg);
#endif   
      }
      else{
        Serial.println("mySensor = NULL");
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
   // mRemotePort=5416;
#ifdef DEBUG
    SERIAL_LOG.print(LOG_HEADER);
    SERIAL_LOG.print("    sendMsg :");
    SERIAL_LOG.printf(" send to %s, port %d\t",  broadcastIp.toString().c_str(), localUdpPort);
    SERIAL_LOG.println((char *)inData);
#endif  
#ifndef DEBUG
    SERIAL_LOG.println((char *)inData);
#endif
    Udp.beginPacket(broadcastIp, localUdpPort);
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

