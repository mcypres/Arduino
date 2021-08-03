





  #define SSID        "wifitelsur_MWAC"
  #define PASSWORD    "0019C70D4077X"
  #define HOST_NAME   "192.168.4.2"
  #define HOST_PORT   5416
  #define REM_PORT  5416
  #define LOG_BAUDRATE 115200
  #define ESP_BAUDRATE 115200
  #define DELAY 100
  #define CIRC_BUFF_LENGH 256
  #define MAX_TRY 100000
  #define WAIT_CHAR_TIME_OUT 1000
  #define TIME_ERR 60000
//relais
#define RELAIS_SIZE 8
#define LOG_BAUDRATE 115200
#define TIME_OUT 1500

int relais[RELAIS_SIZE]={46,47,48,49,50,51,52,53};
//int relais[RELAIS_SIZE]={13,47,48,49,50,51,52,53};
long timeout[RELAIS_SIZE]={0,0,0,0,0,0,0,0};

  long currentTime=0;
  long loopNum=0;
  long nbErr=0;
  long errTime;
  long sendedByte;
  /*******************************************************************/
  void setup() {
  /*******************************************************************/  
   //Set wifi mode：
   String command;
   Serial3.begin(ESP_BAUDRATE);
   Serial.begin(LOG_BAUDRATE);
   Serial.println("setup Start");
   Serial3.println("ATE1");
   //reset esp8266
   Serial.println("sended AT+RST");
   Serial3.println("AT+RST");
   delay(1500);
   sendReponse();//Response：OK
  //Serial.println("ATE0");
  Serial3.println("ATE1"); 
  //mode=3
  Serial.println("sdended AT+CWMODE=3");
  Serial3.println("AT+CWMODE=3"); // softAP+station mode
  
  sendReponse();//Response：OK
  
  //Connect to router
  
 delay(1000);
  Serial.println("AT+CWJAP=\""+(String)SSID+"\",\""+PASSWORD+"\"");
  Serial3.println("AT+CWJAP=\""+(String)SSID+"\",\""+PASSWORD+"\""); // ssid and password of router
  sendReponse();//Response：OK
  
  //Query device’s IP
  
  delay(5000);
  Serial.println("send  AT+CIFSR");
  Serial3.println("AT+CIFSR");
  
  //Response：+CIFSR:STAIP,"192.168.101.104" // IP address of ESP8266 station
  sendReponse();
  //PC connects to the same router which ESP8266 connects to. Using a network tool (eg: ”NetAssist.exe”) on the computer to create UDP .
  
  //Enable multiple connection
  
 // Serial.println("send  AT+CIPMUX=1");
 // Serial3.println("AT+CIPMUX=1");
 // sendReponse();//Response：OK
  
  //Create a UDP transmission, for example, id is 4.
  
  Serial.println("send  AT+CIPSTART=\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT);
  Serial3.println("AT+CIPSTART=\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT);
  
  sendReponse();//Response：4,CONNECT OK
  // get status
  Serial.println("sended AT+CIPSTATUS");
  Serial3.println("AT+CIPSTATUS");
   sendReponse();
  /*Note：
  "192.168.101.110",8080 here is the remote ip and port of UDP transmission which create on PC in step 4;
  1112 is the local port of ESP8266, user-define, if user does not define it, it will be a random value;
  0 means destination peer entity of UDP will not change. For example, in this case, if another PC also creates a UDP entity and sends data to ESP8266 port 1112, ESP8266 can receive these data, but when we send data with command “AT+CIPSEND=4,X”, it will still be sent to the first PC. If this parameter is not 0, it will send to the new PC. 
  */
 // Serial3.println("ATE0"); // echo off
 
 Serial.println("AT+CIPMODE=1");
  Serial3.println("AT+CIPMODE=1");
 
 Serial.println("AT+CIPSEND");
  Serial3.println("AT+CIPSEND");
   sendReponse();
  delay(1000);
  //init variables
  
  }
  /*******************************************************************/
  void loop() {
  /*******************************************************************/
    if (Serial3.available()>0) {
    int inByte = Serial3.read();
    Serial.write(inByte);
  }

  // read from port 0, send to port 1:
  if (Serial.available()>0) {
    int inByte = Serial.read();
    Serial3.write(inByte);
    Serial.write(inByte);
  }
  }
  void sendReponse(){
   
    
    while (Serial3.available()>0){
      int inByte = Serial3.read();
      Serial.write(inByte);
    }
  }
