boolean xxx;


#define ESP_RESET_PIN  7
#define ESP_READY_PIN 6

#define RECEIVE_USB
#define BAROMETRE
#define RS232_2
#define RS232_3

//#define WIFI
#define OPTOGROUP
#define ANALOG_GROUP
//#define LOGICAL_OUT
#define DEBUG

#include "Logger.h"
#include <AsyncrSerial.h>
#include <Arduino.h>
#include <MqttInterfaceArduino.h>
#include "SensorSerialNmeaRmc.h"
#include "SensorSerialNmeaHdm.h"
#ifdef BAROMETRE
#include <SensorBmp183.h>
#endif


#ifdef LOGICAL_OUT
#include "DigitalOutput.h"
#endif

//#include <Adafruit_Sensor.h>
#ifdef OPTOGROUP
#include "DigitalInput.h"
#endif
#ifdef ANALOG_GROUP
#include "SensorAnalogGroupInput.h"
#endif


#include <Wire.h>

#define WIFI_HEADER F("***ESP")
#define DECLINAISON_DEG 7
#define DECLINAISON_MIN 59
#define TIME_INTERVAL_BAROMETRE 30000
#define TIME_INTERVAL_GIROUETTE 3000
#define TIME_INTERVAL_OPTO_INPUT 5500
#define TIME_INTERVAL_ANALOG_GROUP 1000
#define TIME_INTERVAL3 150
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define ANALOG_PIN 0
#define MAX_VALUE 360

#define FILTERCOUNT 30

#define X_OFFSET 0
#define Y_OFFSET 0
#define Z_OFFSET 0
#define SPEED_COEF 1

#define SSID        "Bilbo_main"
#define PASSWORD    "marccypres"


// serial ports

//#define LOG_BAUDRATE 9600
//#define ESP_BAUDRATE 9600

#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200


#define RS232_2_BAUDRATE 38400
//#define RS232_3_BAUDRATE 38400
#define RS232_3_BAUDRATE 4800

#define OPTOGROUP_NUMBER 1
#define RMC_NB_FIELD 11

#ifdef BAROMETRE
SensorBmp183* sensorBMP183;
#endif
#ifdef GIROUETTE
SensorHMC5883* mySensor2;
#endif

#ifdef LOGICAL_OUT
DigitalOutput * mDigit;
#endif

//************************************************

//Variables globales

//************************************************

MqttInterfaceArduino * serialInt;
char    machineName[] = "YachtMain";
Logger *myLogger;

static long timeCounter;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

#ifdef OPTOGROUP
DigitalInput *  ptOptoInputGroup;
int optoInput[] = {22, 23, 24, 25, 26, 27, 28, 29};
unsigned long mOptoInputInterval = 0;
#endif

int relais[] = {46, 47, 48, 49, 50, 51, 52, 53};


unsigned long nextOptoInput = 0;
unsigned long nextTimeBaro = 0;
unsigned long nextTimeAnemo = 0;
unsigned long nextTimeAnalogGroupInput = 0;
unsigned long lastTimeRs232;


String inData2;// input buffer rs232 port2
String inData3;// input buffer rs232 port3

String* ptMgWifiIn = NULL;
String msgWifiOut = "";
int doneCount = 0;
unsigned long lastTime = 0;
boolean espReady = false;


#ifdef ANALOG_GROUP
SensorAnalogGroupInput * mInput;
SensorAnalogGroupInput * mInput2;
#endif

int filterCount = 0;

HardwareSerial *ptUSB;
HardwareSerial *ptInputSer2;
HardwareSerial *ptInputSer3;
HardwareSerial *ptEspSer;

static AsyncrSerial*  asyncrLogSer;
static AsyncrSerial*  asyncrInputSer2;
static AsyncrSerial*  asyncrInputSer3;
static AsyncrSerial*  asyncrEspSer;


SensorSerialNmea* ptSensorSerialNmeaHdm;
SensorSerialNmea* ptSensorSerialNmeaRmc;

/**********************************************************************/
void setup() {

  /**********************************************************************/
  // open serial

  ptUSB = &Serial;
  ptUSB->begin(LOG_BAUDRATE); //USB
  myLogger = new Logger(ptUSB, machineName,N);

  ptInputSer2 = &Serial2;
  ptInputSer2->begin(RS232_2_BAUDRATE);

  ptInputSer3 = &Serial3;
  ptInputSer3->begin(RS232_3_BAUDRATE);

  // ptEspSer = &Serial;
  ptEspSer = &Serial1;
  ptEspSer->begin(ESP_BAUDRATE);     //WIFI

 
  // serial
  // Serial.begin(LOG_BAUDRATE);
  // ptUSB->begin(LOG_BAUDRATE);     //USB
  //  Serial2->begin(RS232_2_BAUDRATE); //rs232
  // Serial2->begin(RS232_3_BAUDRATE); //rs232
  //  ptEspSer->begin(ESP_BAUDRATE);     //WIFI
   asyncrLogSer=new AsyncrSerial(ptUSB,myLogger);
   asyncrInputSer2=new AsyncrSerial(ptInputSer2,myLogger);
   asyncrInputSer3=new AsyncrSerial(ptInputSer3,myLogger);
   asyncrEspSer=new AsyncrSerial(ptEspSer,myLogger);
  _LOG_PRINT(M, F("Setup start"), "***********************************************************************************");

  /**************************************************************************************************************/

  serialInt = new MqttInterfaceArduino (ptEspSer, myLogger, SSID, machineName,ESP_READY_PIN);
  int pos = 0;
  boolean ledBuildIn = false;

  ptSensorSerialNmeaRmc=new SensorSerialNmeaRmc( "Serial2/GPS",
                                                ptEspSer,
                                                myLogger,
                                                serialInt);

                                                
  ptSensorSerialNmeaHdm=new SensorSerialNmeaHdm( "Serial3/Compas",
                                                ptEspSer,
                                                myLogger,
                                                serialInt);

  pinMode(ESP_READY_PIN, INPUT);
  pinMode(ESP_RESET_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  /**************************************************************************************************************/

  // initialisation des entree logiques
  //______________________________________

#ifdef OPTOGROUP
  ptOptoInputGroup = new  DigitalInput("Digital Input",ptEspSer, myLogger, sizeof(optoInput) / sizeof(int), optoInput, OPTOGROUP_NUMBER, serialInt);
#endif

  /**************************************************************************************************************/

  // initialisation des entree analogiques
  //______________________________________
#ifdef ANALOG_GROUP
  const int pinNumber[] = {8, 9, 10, 11};
  const int pinNumber2[] = {12, 13, 14, 15};
  const float scale[] = {66, 66, 66, 66};
  const float scale2[] = {0.653, 1.2995, 1.2995, 1.2995};
  const float scale3[] = {2.84, 2.84, 2.84, 2.84};
  const int offset[] = {512, 512, 512, 512};
  const int offset2[] = {0, 0, 0, 0};
  const float refVoltage = 1000;      // in mV

  //const float refVoltage = 2519

    _LOG_PRINT(D, F(" ESP not Ready" ), F("" ));;      // in mV
  const float refVoltage2 = 2519;    // in mV !!!!!!!!!!!!!!!!!!!

  // const int refSource=DEFAULT;
  //mInput = new SensorAnalogGroupInput("Analogique Input",ptEspSer, myLogger, 'A', INT, pinNumber, scale, offset, refVoltage, EXTERNAL,serialInt);
  
  mInput = new SensorAnalogGroupInput("Analogique Input1",ptEspSer, myLogger, 'A', INT, pinNumber, scale3, offset2, refVoltage, DEFAULT,serialInt);
  mInput2 = new SensorAnalogGroupInput("Analogique Input",ptEspSer, myLogger, 'A', VOL, pinNumber2, scale3, offset2, refVoltage, DEFAULT,serialInt);
#endif
  /**************************************************************************************************************/

  // reception des sorties logique
  //_______________________________
#ifdef LOGICAL_OUT
  mDigit = new DigitalOutput(myLogger, 8, relais);
#endif

  /**************************************************************************************************************/
  // barometre bmp180
  //__________

#ifdef BAROMETRE
  _LOG_PRINT(M, F("setup"), F("Setup start BAROMETRE"));

  sensorBMP183 = new SensorBmp183("Barometre",ptEspSer, myLogger, serialInt);
  if (sensorBMP183->getError()) {
    _LOG_PRINT(M, F("erreur ouverture SensorBmp183 "), sensorBMP183->getErrorMsg());
    delete sensorBMP183;
    sensorBMP183 = NULL;
  }
  else {
    _LOG_PRINT(M, F("setup"), F("ouverture SensorBmp183 OK " ));
  }
#endif
  delay(500);

  /**************************************************************************************************************/




  //reset esp8266
  //____________
#ifdef WIFI

  // reset esp8266

 // digitalWrite(ESP_RESET_PIN, HIGH);
  //digitalWrite(LED_BUILTIN, HIGH);
 /* delay(100);
  digitalWrite(ESP_RESET_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(ESP_RESET_PIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
*/
  _LOG_PRINT(M, F("setup"), F("waiting esp start"));
  _LOG_PRINT(M, F("ESP_READY_PIN "),ESP_READY_PIN);
    //while (espReady == false) {
    while (digitalRead(ESP_READY_PIN) == LOW) {
      _LOG_PRINT(D, F("ESP_READY_PIN"),digitalRead(ESP_READY_PIN));
     if (ledBuildIn) {
       digitalWrite(LED_BUILTIN, HIGH);
       ledBuildIn = false;
     } else {
       digitalWrite(LED_BUILTIN, LOW);
       ledBuildIn = true;
     }
    /* do{
       ptMgWifiIn=asyncrEspSer.getMessage();
       if (ptMgWifiIn!=NULL){

         ptUSB->print(*ptMgWifiIn);

         if (ptMgWifiIn->indexOf(F("Broadcasting")) >=0) {
           espReady = true;
           _LOG_PRINT(M,F("setup esp"),F(" esp started"));
         }
         else{
           ptUSB->print("******setup esp received ****");
           ptUSB->println(*ptMgWifiIn);
         }
       }
     }while (ptMgWifiIn =NULL);*/
      delay(500);
    }

  digitalWrite(LED_BUILTIN, LOW);


#endif  //WIFI

  /**************************************************************************************************************/

#ifdef OPTOGROUP
  _LOG_PRINT(M, F("PTOGROUP  TIME_INTERVAL_OPTO_INPUT "), TIME_INTERVAL_OPTO_INPUT);

#endif
#ifdef ANALOG_GROUP
  _LOG_PRINT(M, "ANALOG_GROUP TIME_INTERVAL_ANALOG_GROUP ", TIME_INTERVAL_ANALOG_GROUP);

#endif
#ifdef RECEIVE_USB
  _LOG_PRINT(M, "setup", "RECEIVE_USB");
#endif
#ifdef BAROMETRE
  _LOG_PRINT(M, "setup", F("BAROMETRE"));
#endif
#ifdef GIROUETTE
  _LOG_PRINT(M, F("setup"), F("GIROUETTE "));
#endif
#ifdef RS232_2
  _LOG_PRINT(M, F("RS232_2"), RS232_2_BAUDRATE);
#endif
#ifdef RS232_3
  _LOG_PRINT(M, F("RS232_3"), RS232_3_BAUDRATE);
#endif
#ifdef WIFI
   _LOG_PRINT(M, F("WIFI"), F("active "));
#endif
 // _LOG_PRINT(M, F("setup"), F("******END********* "));
 Serial.println("setup end");
}


/**********************************************************************/

void loop() {
  char*  ptMsg;
  //Serial.println("loop start");
  /**********************************************************************/
  //  "start loop"
  //--------------------------------------------------------
  // reception
   // _LOG_PRINT(M, F("loop"), F("****loop start ********* "));

#ifdef WIFI
  char* ptMsgEspSer = asyncrEspSer->getMessage();
  if (ptMsgEspSer != NULL) {
#ifdef LOGICAL_OUT

    // positionement des ports logiques
    mDigit->write(*ptMsgEspSer);
#endif //LOGICAL_OUT
    ptUSB->print("ptMsgEspSer : ");
    ptUSB->println(ptMsgEspSer);

  }
#endif //WIFI

    _LOG_PRINT(V, F("loop"), F("****end wifi ********* "));

  //______________________________________________________

#ifdef RECEIVE_USB
  char * ptMsgLogSer = asyncrLogSer->getMessage();
  if (ptMsgLogSer != NULL) {

   // 
       //ptEspSer->print("ptMsgLogSer"); //a revoir

    //ptEspSer->print(ptMsgLogSer); //a revoir
  }
  else {
   // _LOG_PRINT(V, F("ptMsgLogSer"), " is NULL");
  }
#endif
  //___________________________________________________________

  // read from port  ser2:
  //_____________________
    _LOG_PRINT(V, F("loop"), F("****ser2********* "));

#ifdef RS232_2

  char * ptMsgSer2 = asyncrInputSer2->getMessage();
  if (ptMsgSer2 != NULL ) {
  // if (ptMsgSer2 != NULL && digitalRead(ESP_READY_PIN)!=1) {
   
    _LOG_PRINT(M, F("asyncrInputSer2  RS232_2 ptMsgSer2 = "), ptMsgSer2 );
    ptSensorSerialNmeaRmc->publish(ptMsgSer2);
  }
  
#endif
  //___________________________________________________________

  // read from port  ser3:
  //_____________________

#ifdef RS232_3
  char* ptMsgSer3 = asyncrInputSer3->getMessage();
  _LOG_PRINT(V, F("asyncrInputSer3  RS232_3 ptMsgSer3 = "), ptMsgSer3 );
  if (ptMsgSer3 != NULL ) {
      //if (ptMsgSer3 != NULL && digitalRead(ESP_READY_PIN)!=1) {

      _LOG_PRINT(M, F("ptSensorSerialNmeaHdm->publish = "), ptMsgSer3 );
        ptSensorSerialNmeaHdm->publish(ptMsgSer3);
  }
 

#endif
#define  NO_LOG
  /**************************************************************************************************************/
  if(digitalRead(ESP_READY_PIN)!=1){

    _LOG_PRINT(D, F("ESP not Ready : ESP_READY = "),digitalRead(ESP_READY_PIN));
  }
  else{
   // _LOG_PRINT(M, F(" ESP  Ready" ), F("" ));
   
#ifdef BAROMETRE
    if (lastTime + TIME_INTERVAL_GIROUETTE < millis()) {
  
      timeCounter = 0;
      lastTime = millis();
      //------------------------------------------------------------
  
      //barometre
      //__________
  
    if(sensorBMP183!=NULL){
  
        _LOG_PRINT(V, F(" start baro time "), millis());
        sensorBMP183->getData();
        _LOG_PRINT(V, "  baro  ", "sensorBMP183->getData");
#ifdef NMEA
        sensorBMP183->toNmea();
    
        _LOG_PRINT(V, F("  baro  "), "sensorBMP183->toNmea");
#endif
        sensorBMP183->publish();
 
  
    }
    else {
      //timeCounter++;
    }
  }
  #endif  //BAROMETRE
   
    /**************************************************************************************************************/
  
    // lecture des entrees logiques
    //______________________________________
  
  #ifdef OPTOGROUP
    if (mOptoInputInterval + TIME_INTERVAL_OPTO_INPUT < millis()) {
  #ifdef NMEA
      ptOptoInputGroup->toNmea();
  #endif
      ptOptoInputGroup->publish();
      mOptoInputInterval = millis();
    }
#endif //OPTOGROUP


 
    /**************************************************************************************************************/
  

  
    // lecture des entrees analogiques
    //______________________________________


  
#ifdef ANALOG_GROUP

    if (nextTimeAnalogGroupInput + TIME_INTERVAL_ANALOG_GROUP < millis()) {
  
      _LOG_PRINT(V, F(" ANALOG_GROUP" ), F("start" ));
#ifdef NMEA
      // mInput->toNmea();
      //mInput2->toNmea();
#endif   
      mInput2->publish();
      nextTimeAnalogGroupInput = millis();
      _LOG_PRINT(V, F(" ANALOG_GROUP" ), F("end" ));

    }
    
#endif  //ANALOG_GROUP
  
  
  }
#undef NO_LOG
}
  
  /*****************************************************************************/
