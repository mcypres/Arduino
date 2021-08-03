//#include <CircBuf.h>

//#include <Udpservices.h>



  #define SSID        "wifitelsur_MWAC"
  #define PASSWORD    "0019C70D4077X"
  #define HOST_NAME   "192.168.4.2"
  #define HOST_PORT   5416
  #define REM_PORT  5416
  #define LOG_BAUDRATE 115200
   #define ESP_BAUDRATE 115200
  #define DELAY 1000
  #define CIRC_BUFF_LENGH 256
  long currentTime=0;
  
  //CircBuf inputBuff(CIRC_BUFF_LENGH);

  
  void setup() {
    
   //Set wifi mode：
   String command;
   Serial3.begin(ESP_BAUDRATE);
   Serial.begin(LOG_BAUDRATE);
   Serial.println("setup Start");
   //Serial3.println("ATE1");
   Serial.println("sended AT+RST");
   Serial3.println("AT+RST");
   delay(1500);
   sendReponse();//Response：OK
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
  Serial.println("send  AT+CIPMUX=1");
  Serial3.println("AT+CIPMUX=1");
  sendReponse();//Response：OK
  //Create a UDP transmission, for example, id is 4.
  Serial.println("send  AT+CIPSTART=4,\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT+",0");
  Serial3.println("AT+CIPSTART=4,\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT+",0");
  
  sendReponse();//Response：4,CONNECT OK
  Serial.println("sended AT+CIPSTATUS");
  Serial3.println("AT+CIPSTATUS");
   sendReponse();
  /*Note：
  "192.168.101.110",8080 here is the remote ip and port of UDP transmission which create on PC in step 4;
  1112 is the local port of ESP8266, user-define, if user does not define it, it will be a random value;
  0 means destination peer entity of UDP will not change. For example, in this case, if another PC also creates a UDP entity and sends data to ESP8266 port 1112, ESP8266 can receive these data, but when we send data with command “AT+CIPSEND=4,X”, it will still be sent to the first PC. If this parameter is not 0, it will send to the new PC. 
  */
 // Serial3.println("ATE0"); // echo off
  Serial.println("setup end");
  }
  
  void loop() {
   /* Send data
  AT+CIPSEND=4,5 // Send 5 bytes to transmission NO.4
  Response > DGFYQ // enter the data, no CR
  SEND OK
  Note: If the number of bytes sent is bigger than the size defined (n), will reply busy, and after sending n number of bytes, reply SEND OK.*/
  if(currentTime+DELAY<millis()){
    
    currentTime=millis();
 
    String msg="$PAMPR,01023.00,hPa,23.700, C*62";
    Serial3.println("AT+CIPSEND=4,"+msg.length()+msg);
   //Serial3.println(msg.length());
   //delay(5000);
   // sendReponse();
    long startTime=millis()+2000; 
  //  while ((Serial3.available()>0)&&(startTime>millis())){}
  //  Serial.println("---- available");
    char c;
 //   Serial.print(msg+"\n---- received ---");
 //   do{
    //  c=Serial3.read();
  //    if (c!=-1){
  //      Serial.print(c);
  //    }
 //   }while (c!='>'&&(startTime>millis()));
 //   Serial.print(c);
 //   Serial.println("---");
  //  Serial3.print(msg);
    delay(1000);
    String msg2="$PAMWV,62.30,R,247.30,N*74";
   Serial3.println("AT+CIPSEND=4,"+msg2.length());
   //Serial3.println(msg2.length());
  // delay(5000);
   // sendReponse();
    startTime=millis()+2000; 
    while ((Serial3.available()>0)&&(startTime>millis())){}
  //  Serial.println("---- available");
   // char c;
    Serial.print(msg2+"\n---- received ---");
    do{
      c=Serial3.read();
      if (c!=-1){
        Serial.print(c);
      }
    }while (c!='>'&&(startTime>millis()));
 //   Serial.print(c);
    Serial.println("---");
    Serial3.print(msg2);
  }
 //   sendReponse();
 //   Serial.println("loop wait rep");
   // currentTime=millis();
    ///if (Serial3.available()>0){
     // Serial.print("duration :");
     // Serial.println(millis()-currentTime) ;    
     // Serial.println(Serial3.readStringUntil('\n'));
     
  //  }
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

