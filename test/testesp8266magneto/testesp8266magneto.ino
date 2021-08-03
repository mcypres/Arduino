#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SensorHMC5883.h>


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_GIROUETTE 1000
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0
#define LOG_SERIAL Serial

//#define INADDR_NON 0,0,0,0
const char* ssid = "wifitelsur_MWAC";
const char* password = "0019C70D4077X";

WiFiUDP Udp;
unsigned int localUdpPort = 5416;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacekt[] = "Hi there! Got the message :-)";  // a reply string to send back
IPAddress remoteIp;
boolean firstIp=false;
/* Assign a unique ID to this sensor at the same time */
//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
SensorHMC5883* mySensor;
int sensorValue = 0;  // variable to store the value coming from the sensor
float cap=0;


// compteur girouette
volatile unsigned long GanemoCount;
#define ANEMO_PIN 14
#define ANEMO_INTERUPT 1
unsigned long lastTime=0;
void setup()
{
  Serial.begin(115200);
  Serial.println();
 
// attachInterrupt(ANEMO_INTERUPT, anemoInterupt, RISING);
 
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  
    /* Initialise the sensor */
//if(!mag.begin())
  //{
    /* There was a problem detecting the HMC5883 ... check your connections */
//    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
//    while(1);
//  }*/
  
  /* Display some basic information on this sensor */
//  displaySensorDetails();
  LOG_SERIAL.println("Setup start GIROUETTE");
  mySensor = new SensorHMC5883(DECLINAISON_DEG, DECLINAISON_MIN,X_OFFSET,Y_OFFSET,Z_OFFSET, 12345);
  if (mySensor->getError()) {
    LOG_SERIAL.println("ouverture SensorHMC5883 " + mySensor->getErrorMsg());
    delete mySensor;
    mySensor = NULL;
  }
  else {
    LOG_SERIAL.println("girouette ok");
    LOG_SERIAL.println(mySensor->getSensorDetail());
  }
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // send back a reply, to the IP address and port we got the packet from
   // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
   remoteIp=Udp.remoteIP();
   firstIp=true;
   Udp.beginPacket(Udp.remoteIP(), 5416);
    Udp.write(replyPacekt);
    Udp.endPacket();
  }
   // anemo girouette
  delay(100);
  if(lastTime+TIME_INTERVAL_GIROUETTE<millis()){ 
    lastTime=millis(); 
      if (mySensor != NULL) {
        String msg;
        //LOG_SERIAL.println("mySensor start");
        mySensor->getData();
        //msg=mySensor->toString();
        msg=mySensor->toNmea();
        if(firstIp==true){
          char cmsg[201];
          msg.toCharArray(cmsg,msg.length());
          // LOG_SERIAL.print("mySensor2 send :");
          Udp.beginPacket(remoteIp, 5416);
          Udp.write(cmsg);
          Udp.endPacket();
          
        }
        LOG_SERIAL.println(msg);
      }
      else{
        LOG_SERIAL.println("mySensor = NULL");
      }
    }
  
  }
  
void anemoInterupt() {
  GanemoCount++;
}



