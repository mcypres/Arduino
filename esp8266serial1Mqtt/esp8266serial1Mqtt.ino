
# define BUFF_LENGTH 250

#define MQTT_USER ""
#define MQTT_PASS ""

#define MSG_SIZE 200
#define TOPIC_SIZE 50
#define READY_PIN 12


#include <AsyncrSerial.h>
AsyncrSerial*  asyncrInputSer;
//WiFi Connection configuration

char ssid[] = "BilboNet";     //  le nom du reseau WIFI
char password[] = "marccypres";  // le mot de passe WIFI

//mqtt server

char mqtt_server[] = "192.168.4.1";  //adresse IP serveur

static boolean led;
char    machineName[]="EspMain";

#include <Logger.h>
HardwareSerial *ptLogOutputSer;
HardwareSerial *ptOutputSer;

Logger* myLogger;

char  inData[BUFF_LENGTH];          // input buffer rs232

#include <MqttInterfaceEsp8266.h>
MqttInterfaceEsp8266 * mqttIterface;

/***********************************************************/


void setup() {
  pinMode(READY_PIN, OUTPUT);
  digitalWrite(READY_PIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  led = true;
  ptLogOutputSer=&Serial1;
  ptOutputSer= &Serial;
  ptOutputSer->begin(115200);
  ptOutputSer->println("------test Serial-----");
  ptLogOutputSer->begin(74880);
  myLogger= new Logger(ptLogOutputSer,"machineName",W);  

  asyncrInputSer = new AsyncrSerial(ptOutputSer, myLogger);
  
  //ptLogOutputSer->println("*****************************");
  //ptLogOutputSer->println("*****************************");
  
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
    
    delay(50);
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
  
  _LOG_PRINT(V,"end ","setup");
}

void loop() {
  if (!mqttIterface->isConnected()) {
    
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(READY_PIN, LOW);
 
    mqttIterface->connect();
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(READY_PIN, HIGH);
 
  }
  mqttIterface->loop();
  _LOG_PRINT(V,"after  mqttIterface->loop","");
  String * ptMsg=NULL ;  //= &String("No message");
  ptMsg = asyncrInputSer->getMessage();
  if (ptMsg != NULL) {
    _LOG_PRINT(M,"received ",*ptMsg);
    mqttIterface->publishSerial(ptMsg);
   
    _LOG_PRINT(D,"pulished",millis());

  }
  yield();
  //delay(50);

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
