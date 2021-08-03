                                                               


//#define NO_LOG

//#include <MqttInterfaceEsp8266.h>
#include <Logger.h>
#include <ESP8266WiFi.h>


# define BUFF_LENGTH 250

#define MQTT_USER ""
#define MQTT_PASS ""

#define MSG_SIZE 200
#define TOPIC_SIZE 50
#define READY_PIN 12

//WiFi Connection configuration

char ssid[] = "BilboNet";     //  le nom du reseau WIFI
char password[] = "marccypres";  // le mot de passe WIFI

//mqtt server

char mqtt_server[] = "192.168.4.1";  //adresse IP serveur

static boolean led;
char    machineName[]="EspMain";

// Serial I/O

HardwareSerial *ptOutputSer;
HardwareSerial *ptLogOutputSer;
Logger* myLogger;

char  inData[BUFF_LENGTH];          // input buffer rs232

#include <MqttInterfaceEsp8266.h>
MqttInterfaceEsp8266 * mqttIterface;

/********************************************************************************************/
void setup(){
  pinMode(READY_PIN, OUTPUT);
  digitalWrite(READY_PIN, LOW);
 
  ptOutputSer=&Serial;
  ptOutputSer->begin(115200);
  Serial.println("__________Serial________________");
  
  ptLogOutputSer= &Serial1;
  ptLogOutputSer->begin(74880);
  Serial1.println("__________Serial1________________");
  ptLogOutputSer->println("\n\n******ptLogOutputSer***NL");
  ptLogOutputSer->println("\n*****ptLogOutputSer2**");
  
  Serial1.println("__________Serial1____B___________");
  Serial.println("__________Serial________________");
  myLogger= new Logger(ptLogOutputSer,machineName,E);
  _LOG_PRINT(M,"Setup ","Start");

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
  ptLogOutputSer->println("\n");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, led);

    if (led==true){
      led=false;
    }
    else{
      led=true;
    }
    
    delay(500);
    ptLogOutputSer->print(".");
  }
  led=false;
  digitalWrite(LED_BUILTIN, led);
  ptLogOutputSer->println("Connected");
  mqttIterface= new MqttInterfaceEsp8266(ptOutputSer,myLogger,machineName,ssid);
  mqttIterface->setServer(mqtt_server, 1883);
  _LOG_PRINT(M,"Setup ","End");

 // set the message receive callback
  mqttIterface->setCallback(callback);
  delay(500);
  ptOutputSer->println("Broadcasting");// pour signaler a mega reset ok
 
  digitalWrite(READY_PIN, HIGH);
  delay(500);
}
 
/***************************************************************************************************************************/
void loop(){
 
  if (!mqttIterface->isConnected()) {
    
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(READY_PIN, LOW);
 
    mqttIterface->connect();
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(READY_PIN, HIGH);
 
  }
  mqttIterface->loop();
  mqttIterface->readSerial();
    ptLogOutputSer->println("\n*****ptLogOutputSer loop**");

}
  
 
/********   ecouter SERIAL_ARDUINO envoyer sur MQTT *********/

void sendMsg(char* inData) {

  _LOG_PRINT(V,"sendMsg",inData);
     mqttIterface->publish("EspMain/log", inData);
 
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  ptOutputSer->print("Message arrived [");
  ptOutputSer->print(topic);
  ptOutputSer->print("] ");
  for (int i = 0; i < length; i++) {
    ptOutputSer->print((char)payload[i]);
  }
  ptOutputSer->println();

}
