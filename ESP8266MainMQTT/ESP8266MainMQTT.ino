                                                               


//#define NO_LOG
#include"Logger.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>

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
#define MQTT_USER ""
#define MQTT_PASS ""

#define MSG_SIZE 200
#define TOPIC_SIZE 50

#include<MqttInterface.h>

#define START_CHAR '#'
#define SEPA_CHAR '&'
#define END_CHAR '\0'

//WiFi Connection configuration

char ssid[] = "BilboNet";     //  le nom du reseau WIFI
char password[] = "marccypres";  // le mot de passe WIFI

//mqtt server

char mqtt_server[] = "192.168.4.1";  //adresse IP serveur
WiFiClient espClient;
PubSubClient MQTTclient(espClient);

static boolean led;
char    machineName[]="EspMain";

// Serial I/O

HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
Logger* myLogger;


//char incomingPacket[BUFF_LENGTH];   // buffer for incoming packets
char  inData[BUFF_LENGTH];          // input buffer rs232
int buffPos = 0;

int mRemotePort=0;
boolean firstIp = false;

//WiFiServer server(localUdpPort);

enum StateWaiting {
  start,
  commande,
  endTopic,
  endMessage
};
static StateWaiting state=start;
static char buffMsg[MSG_SIZE];
static char buffTopic[TOPIC_SIZE];


/********************************************************************************************/
void setup(){

  ptOutputSer=&SERIAL_ARDUINO;
  ptLogOutputSer= &SERIAL_LOG;
 // ptOutputSer->begin(115200);
  
  ptLogOutputSer->begin(115200);
  myLogger= new Logger(ptLogOutputSer,machineName,D);
  _LOG_PRINT(D,"Setup ","Start");


  pinMode(LED_BUILTIN, OUTPUT);
  led = true;
  
  //MQTT
  
  digitalWrite(LED_BUILTIN, HIGH);
  // Conexion WIFI
  _LOG_PRINT(M,"connecting to  ",ssid);
  WiFi.disconnect();
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  MQTTclient.setServer(mqtt_server, 1883);
  _LOG_PRINT(M,"Setup ","End");

 // set the message receive callback
  MQTTclient.setCallback(callback);
 delay(100);
  ptOutputSer->println("Broadcasting");// pour signaler a mega reset ok
}
 
/***************************************************************************************************************************/
void loop(){
  readSerial();
  if (!MQTTclient.connected()) {
    digitalWrite(LED_BUILTIN, LOW);
    reconnect();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  MQTTclient.loop();
}
  
 
/********   ecouter SERIAL_ARDUINO envoyer sur MQTT *********/

void sendMsg(char* inData) {

  _LOG_PRINT(V,"sendMsg",inData);
     MQTTclient.publish("Bilbo/girouette", "Message a envoyer");
 
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  ptOutputSer->print("Message arrived [");
  ptOutputSer->print(topic);
  ptOutputSer->print("] ");
  for (int i = 0; i < length; i++) {
    ptOutputSer->print((char)payload[i]);
  }
  ptOutputSer->println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void readSerial(){
static int posTopic,posMsg;
static char cmd;
  while (ptOutputSer->available() > 0) {
    char car=ptOutputSer->read();
/*  start,
  cmd,
  endTopic,
  endMessage
  */

    switch  (state){
      
      case start:
        if (car==START_CHAR){
          _LOG_PRINT(D,"decript","start");
          state=commande;
          posTopic=0;
          posMsg=0;
        }
        break;

      case commande:
        cmd=car;
        state=endTopic;
        posTopic=0;          
        _LOG_PRINT(D,"commnande= ",cmd);        
        break;
        
      case endTopic:
        
        if (car==SEPA_CHAR){
          buffTopic[posTopic++]='\0';
          _LOG_PRINT(D,"fin topic ",buffTopic);
          if (posTopic>=TOPIC_SIZE){
            state=start;  // cas d erreur on ignore la suite
          } 
          if (cmd='s'){
            _LOG_PRINT(D,"subscribe ",buffTopic);
            MQTTclient.subscribe(buffTopic);
            state=endMessage;
            break;
          }
          if (cmd='u'){
            _LOG_PRINT(D,"unsubscribe ",buffTopic);
            MQTTclient.unsubscribe(buffTopic);
            state=endMessage;
            break;
          }
          buffTopic[posTopic++]=car;
          //state=start;
          break;
        }
        state=endMessage;
        buffTopic[posTopic++]=car;
        break;
        
      case endMessage:
        if (car==END_CHAR){
          buffMsg[posMsg++]='\0';
          _LOG_PRINT(V,"publish topic ",buffTopic);
          _LOG_PRINT(V,"publish msg ",buffMsg);
          digitalWrite(LED_BUILTIN, HIGH);
          MQTTclient.publish(buffTopic, buffMsg);
          digitalWrite(LED_BUILTIN, LOW);
          state=start;
          break;         
        }
        buffMsg[posMsg++]=car;
        if(posMsg>=MSG_SIZE){
          state=start;
        }
        break;
    }       
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) {
    //Serial.print("Attempting MQTT connection...");
    _LOG_PRINT(M,"Attempting MQTT connection...","");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (MQTTclient.connect(clientId.c_str())) {
      //Serial.println("connected");
      _LOG_PRINT(M,"connected","");
      // Once connected, publish an announcement...
    //  client.publish("outTopic", "hello world");
      // ... and resubscribe
//client.subscribe("inTopic");
    } else {
     // Serial.print("failed, rc=");
     // Serial.print(MQTTclient.state());
      _LOG_PRINT(M,"failed, rc= ",MQTTclient.state());
      _LOG_PRINT(M," try again in 5 seconds","");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
