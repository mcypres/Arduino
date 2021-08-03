  #define SSID        "wifitelsur_MWAC"
  #define PASSWORD    "0019C70D4077X"
  //#define SSID        "marc-Aspire-E5-521G"
  //#define PASSWORD    "mazel21"
  #define HOST_NAME   "192.168.4.2"
  #define HOST_PORT   "5416"
  #define HOST_REM_PORT  "5417"

  #define DELAY 10000
  long currentTime=0;
  void setup() {
    
   //Set wifi mode：
   Serial3.begin(115200);
   Serial.begin(115200);
   Serial.println("setup Start");
   // Serial.println("AT+RST");
   Serial3.println("AT+RST");
   delay(500);
   sendReponse();//Response：OK
  // Serial.println("AT+CWMODE=3");
  Serial3.println("AT+CWMODE=3"); // softAP+station mode
  
  sendReponse();//Response：OK
  //Connect to router
  //Serial.println("send  AT+CWJAP=\"wifitelsur_MWAC\",\"0019C70D4077X\"");
  Serial3.println("AT+CWJAP=\"wifitelsur_MWAC\",\"0019C70D4077X\""); // ssid and password of router
  sendReponse();//Response：OK
  //Query device’s IP
  delay(5000);
  //Serial.println("send  AT+CIFSR");
  Serial3.println("AT+CIFSR");
  
  //Response：+CIFSR:STAIP,"192.168.101.104" // IP address of ESP8266 station
  sendReponse();
  //PC connects to the same router which ESP8266 connects to. Using a network tool (eg: ”NetAssist.exe”) on the computer to create UDP .
  //Enable multiple connection
 // Serial.println("send  AT+CIPMUX=1");
  Serial3.println("AT+CIPMUX=1");
  sendReponse();//Response：OK
  //Create a UDP transmission, for example, id is 4.
 // Serial.println("send  AT+CIPSTART=4,\"UDP\",\"192.168.4.2\",5416,5416,0");
  Serial3.println("AT+CIPSTART=4,\"UDP\",\"192.168.4.2\",5416,5416,0");
  
  sendReponse();//Response：4,CONNECT OK
  //Serial.println("AT+CIPSTATUS");
  Serial3.println("AT+CIPSTATUS");
   sendReponse();
  /*Note：
  "192.168.101.110",8080 here is the remote ip and port of UDP transmission which create on PC in step 4;
  1112 is the local port of ESP8266, user-define, if user does not define it, it will be a random value;
  0 means destination peer entity of UDP will not change. For example, in this case, if another PC also creates a UDP entity and sends data to ESP8266 port 1112, ESP8266 can receive these data, but when we send data with command “AT+CIPSEND=4,X”, it will still be sent to the first PC. If this parameter is not 0, it will send to the new PC. 
  */
  
  Serial.println("setup end");
  }
  
  void loop() {
   /* Send data
  AT+CIPSEND=4,5 // Send 5 bytes to transmission NO.4
  Response > DGFYQ // enter the data, no CR
  SEND OK
  Note: If the number of bytes sent is bigger than the size defined (n), will reply busy, and after sending n number of bytes, reply SEND OK.*/
 // if(currentTime+DELAY<millis()){
    
  //  currentTime=millis();
 
 //   String msg="hello";
  //  Serial3.print("AT+CIPSEND=4,");
  //  Serial3.println(msg.length());
  //  sendReponse();
 
 //   Serial3.print(msg);
 //   sendReponse();
 //   Serial.println("loop wait rep");
    currentTime=millis();
    if (Serial3.available()>0){
      Serial.print("duration :");
      Serial.println(millis()-currentTime) ;    
      Serial.print(Serial3.readStringUntil('n'));
     
    }
 // }
  //Receive data:
  //+IPD,4,n:xxxxxxxxxx // received n bytes, data=xxxxxxxxxxx
  //Delete transmission NO.4
  //AT+CIPCLOSE=4
  //Response：4,CLOSED OK
  }
  
  void sendReponse(){
   
    delay(1000);
    if (Serial3.available()>0){
      Serial.print("received :");
    Serial.println(Serial3.readString());
    }
  }

