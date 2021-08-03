
// BONNE VERSION 13/03/2020

//#define NO_LOG
#include"Logger.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NmeaMwv.h>
#define DEBUG
#define DEBUG_IN

# define BUFF_LENGTH 250
#define BROACAST_IP "192.168.4.255"
#define GIROUETE_IP 192,168,4,2
#define MY_IP 192,168,4,1
#define IPV 4
#define HOST_NAME "Bilbo_Main"
#define LOG_HEADER "***bilbo Main***"
#define SERIAL_LOG Serial
#define SERIAL_ARDUINO Serial



char    machineName[]="EspMain";

HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
Logger* myLogger;

const char* ssid = HOST_NAME;
const char* password = "marccypres";

WiFiUDP Udp;
unsigned int localUdpPort = 5416;   // local port to listen on
char incomingPacket[BUFF_LENGTH];   // buffer for incoming packets
char  inData[BUFF_LENGTH];          // input buffer rs232
int buffPos = 0;
IPAddress myIP(192,168,4,1);
IPAddress remoteIp;
IPAddress broadcastIp;
IPAddress gateway(192,168,4,2);
IPAddress submet(255,255,255,0);
int mRemotePort=0;
boolean firstIp = false;

WiFiServer server(localUdpPort);
int heading;
float speed;
NmeaMwv* myNmeaMwv;
/********************************************************************************************/
void setup()
{
  ptOutputSer=&SERIAL_ARDUINO;
  ptLogOutputSer= &SERIAL_LOG;
 // ptOutputSer->begin(115200);
  
  ptLogOutputSer->begin(115200);
  myLogger= new Logger(ptLogOutputSer,machineName,E);
  _LOG_PRINT(M,"Setup ","Start");
  myNmeaMwv= new NmeaMwv(myLogger);
  WiFi.disconnect();  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  _LOG_PRINT(M,"AP IP address: ",myIP);
  server.begin();
  WiFi.config(myIP,gateway,submet);
 
  Udp.begin(localUdpPort);
 // reportSetup();
  

  broadcastIp=GetBroadcastAddress(  WiFi.localIP().toString(), WiFi.subnetMask().toString());
  reportSetup();
  delay(100);
  ptOutputSer->println("Broadcasting");
}
/***************************************************************************************************************************/
void loop()
{

  /*** ecouter UDP envooyer sur SERIAL_ARDUINO *****/
  
  int packetSize = Udp.parsePacket();
 #ifdef RECEIVE_UDP
  if (packetSize)
  {
    // receive incoming UDP packets
    _LOG_PRINT_TAIL(V,"Received ",packetSize,"byte \n");
    _LOG_PRINT(V,"from ",Udp.remoteIP().toString().c_str());   
    _LOG_PRINT(V," port ",Udp.remotePort());
    int len = Udp.read(incomingPacket, BUFF_LENGTH-1);
    if (len > 0){
      incomingPacket[len] = 0;
      ptOutputSer->println(incomingPacket);
    }
    // remoteIp = Udp.remoteIP();
    mRemotePort=Udp.remotePort();
    firstIp = true;
  }
#endif //RECEIVE_UDP

/********   ecouter SERIAL_ARDUINO envoyer sur UDP *********/
#ifdef RECEIVE_SERIAL
  while (SERIAL_ARDUINO.available() > 0) {
    inData[
    buffPos++] = SERIAL_ARDUINO.read();
     yield();
    if (inData[buffPos-1] == '\n' || buffPos == BUFF_LENGTH) {
      inData[buffPos] ='\0';
      _LOG_PRINT(V,"received from Bilbo_main",inData);
      sendMsg(inData);
      buffPos = 0;
      return;
    }

  }
  #endif
  heading=(int)((millis()/1000)%360);
  speed=5;
  sendMsg(myNmeaMwv->toNmea( heading, speed));
}
void sendMsg(char* inData) {

  _LOG_PRINT(V,"sendMsg",inData);
  mRemotePort=5416;
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
/* #ifdef DEBUG  
      SERIAL_LOG.print(LOG_HEADER);     
      SERIAL_LOG.print("ipAdressBytes[");
      SERIAL_LOG.print(i);
      SERIAL_LOG.print("] = ");
      SERIAL_LOG.println(ipAdressBytes[i]);
      
      SERIAL_LOG.print("subnetMaskBytes[");
      SERIAL_LOG.print( i);
      SERIAL_LOG.print("] = ");
      SERIAL_LOG.println(subnetMaskBytes[i]);
      
      SERIAL_LOG.print("subnetMaskBytes[");
      SERIAL_LOG.print(i);
      SERIAL_LOG.print("] ^ 255= ");
      SERIAL_LOG.println(subnetMaskBytes[i] ^ 255);
      
      SERIAL_LOG.print("broadcastAddress[ ");
      SERIAL_LOG.print(i);
      SERIAL_LOG.print("] = ");
      SERIAL_LOG.print(broadcastAddress[i]);
      
#endif*/
     }
   return IPAddress(broadcastAddress[0],broadcastAddress[1],broadcastAddress[2],broadcastAddress[3]);
   }
   
   void split(String* output,String input,char sep,int len){
    int startPos=0;
    int endPos=0;

    _LOG_PRINT(V," split.input= ",input);
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
  void reportSetup(){
    _LOG_PRINT(M,"Local ip = ",WiFi.localIP().toString());
    _LOG_PRINT(M,"subnetMask = ",WiFi.subnetMask().toString());
    _LOG_PRINT(M,"broadcastIp = ",broadcastIp.toString());

    //mSerial.println("Broadcasting");
    _LOG_PRINT(M,"UDP port = ", localUdpPort);
  }
