



#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
# define BUFF_LENGTH 250
#define BROACAST_IP "192.168.4.255"
#define MY_IP 192,168,4,1
#define IPV 4
#define HOST_NAME "bilbomain"
#define LOG_HEADER "***bilbomain***"
#define SERIAL_LOG Serial1
#define SERIAL_ARDUINO Serial

const char* ssid = "wifitelsur_MWAC";
//const char* ssid = "ESP_06C630";
//const char* ssid = "ESP_033607";
const char* password = "0019C70D4077X";
//const char* password = "";
WiFiUDP Udp;
unsigned int localUdpPort = 5416;  // local port to listen on
char incomingPacket[BUFF_LENGTH];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
char  inData[BUFF_LENGTH];// input buffer rs232
int buffPos = 0;
IPAddress myIP(192,168,4,1);
IPAddress remoteIp;
IPAddress broadcastIp;
IPAddress gateway(192,168,4,2);
IPAddress submet(255,255,255,0);
int mRemotePort=0;
boolean firstIp = false;

WiFiServer server(localUdpPort);

/********************************************************************************************/
void setup()
{
  Serial.begin(115200);
  SERIAL_LOG.begin(115200);
  //Serial.println();
 // Serial.printf("Connecting to %s ", ssid);
 
  WiFi.disconnect();
  WiFi.hostname(HOST_NAME);
  server.begin();
  WiFi.config(myIP,gateway,submet);
 
  
  //WiFi.begin(ssid, password);
 /* WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
*/
  Udp.begin(localUdpPort);
  Serial.print(LOG_HEADER);
  Serial.printf("    Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  SERIAL_LOG.print(LOG_HEADER);
  SERIAL_LOG.printf("    Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  //broadcast = ip | ( ~ subnet )
 // broadcastIp= WiFi.localIP()|(~ WiFi.submask());
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
 
 SERIAL_LOG.print(LOG_HEADER);
 SERIAL_LOG.print( "    Local ip = ");
 SERIAL_LOG.println(WiFi.localIP().toString());
 SERIAL_LOG.print(LOG_HEADER);
 SERIAL_LOG.print("    subnetMask = "),
 SERIAL_LOG.println(WiFi.subnetMask().toString());
 SERIAL_LOG.print(LOG_HEADER);
 SERIAL_LOG.print( "    broadcastIp = ");
 SERIAL_LOG.println(broadcastIp.toString());
 SERIAL_LOG.print(LOG_HEADER);
 SERIAL_LOG.printf("    Broadcasting at IP %s, UDP port %d\n", broadcastIp.toString().c_str(), localUdpPort);
}
/***************************************************************************************************************************/
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, BUFF_LENGTH-1);
    if (len > 0){
      incomingPacket[len] = 0;
    }
    // Serial.printf("UDP packet contents: %s\n", incomingPacket);
    Serial.printf("%s\n", incomingPacket);
    SERIAL_LOG.print(LOG_HEADER);
    SERIAL_LOG.print("    received :");
    SERIAL_LOG.printf("%s\n", incomingPacket);
    // remoteIp = Udp.remoteIP();
    mRemotePort=Udp.remotePort();
    firstIp = true;
  }

  while (Serial.available() > 0) {
    inData[buffPos++] = Serial.read();
    SERIAL_LOG.print(LOG_HEADER);
    SERIAL_LOG.print("from mega: ");
    SERIAL_LOG.println(inData);
    if (inData[buffPos-1] == '\n' || buffPos == BUFF_LENGTH) {
      inData[buffPos] ='\0';
 
      sendMsg(inData);
      buffPos = 0;
    }

  }
}
  void sendMsg(char* inData) {
    SERIAL_LOG.print(LOG_HEADER);
    SERIAL_LOG.print("    sendMsg :");
    SERIAL_LOG.println((char *)inData);
    mRemotePort=5416;
   //Serial.printf("send to %s, port %d\n",  broadcastIp.toString().c_str(), mRemotePort);
 
   
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
       /* Serial.printf("ipAdressBytes[ %d] = %d\n", i,ipAdressBytes[i]);
        Serial.printf("subnetMaskBytes[ %d] = %d\n", i,subnetMaskBytes[i]);
        Serial.printf("subnetMaskBytes[ %d] ^ 255= %d\n", i,(subnetMaskBytes[i] ^ 255));
        Serial.printf("broadcastAddress[ %d] = %d\n", i,broadcastAddress[i]);*/
     }
   return IPAddress(broadcastAddress[0],broadcastAddress[1],broadcastAddress[2],broadcastAddress[3]);
   }
   
   void split(String* output,String input,char sep,int len){
    int startPos=0;
    int endPos=0;
    //Serial.print("*********split.input=");
    //Serial.println(input);
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


