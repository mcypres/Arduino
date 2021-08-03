





  #define SSID        "wifitelsur_MWAC"
  #define PASSWORD    "0019C70D4077X"
  #define HOST_NAME   "192.168.4.2"
  #define HOST_PORT   5416
  #define REM_PORT  5416
  #define LOG_BAUDRATE 115200
  #define ESP_BAUDRATE 115200
  #define DELAY 200
  #define CIRC_BUFF_LENGH 256
  #define MAX_TRY 100000
  #define WAIT_CHAR_TIME_OUT 1000
  #define TIME_ERR 60000

  long currentTime=0;
  long loopNum=0;
  long nbErr=0;
  long errTime;
  long sendedByte;
  void setup() {
    
   //Set wifi mode：
   String command;
   Serial3.begin(ESP_BAUDRATE);
   Serial.begin(LOG_BAUDRATE);
   Serial.println("setup Start");
   Serial3.println("ATE1");
   Serial.println("sended AT+RST");
   Serial3.println("AT+RST");
   delay(1500);
   sendReponse();//Response：OK
  //Serial.println("ATE0");
  Serial3.println("ATE0"); //no echo
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
     // Serial3.println("AT+CIPSEND");
      Serial.println("setup end");
      delay(1000);
      nbErr=0; 
      loopNum=0;
      errTime=millis();
      sendedByte=0;
  }
  
  void loop() {
   /* Send data
  AT+CIPSEND=4,5 // Send 5 bytes to transmission NO.4
  Response > DGFYQ // enter the data, no CR
  SEND OK
  Note: If the number of bytes sent is bigger than the size defined (n), will reply busy, and after sending n number of bytes, reply SEND OK.*/
  /*  if(currentTime+DELAY<millis()){
        loopNum++;
        currentTime=millis();
     
        String msg="$$$"+String(loopNum)+" at " +String(millis())+"$$$";
         Serial3.println(msg);
         Serial.println(msg);
      
    }*/
   // read from port 3, send to port 0:
  while (Serial3.available()>0) {
    int inByte = Serial3.read();
    Serial.write(inByte);
  }

  // read from port 0, send to port :
  while (Serial.available()>0) {
    int inByte = Serial.read();
    Serial3.write(inByte);
  }
 
  }
  

 
   void sendReponse(){
   
    delay(1000);
    if (Serial3.available()>0){
      Serial.print("received :");
    Serial.println(Serial3.readString());
    }
  }


  boolean readAnswer(const char clef[]){
    long nbTry=0;//NB_TRY;
    char c;
    int i=0;
    //Serial.println("____________________________clef= "+String(clef));
    long currentTime=millis();
    do{
      while ((Serial3.available()==0) &&((currentTime+WAIT_CHAR_TIME_OUT)<millis())){}
      //Serial.println("nbTry= "+ String(nbTry) +" waited :" +String(millis()-currentTime));
      if ((nbTry>=MAX_TRY) ||((currentTime+WAIT_CHAR_TIME_OUT)<millis())) return false;
      nbTry++;
      
      c=Serial3.read();
       
      if(c!=-1){
       // Serial.println("            *******   i= "+String(i)+ "c= " +c);
       delay(1);
        if(clef[i]==c){
          i++;
        }
      }
    }while(clef[i]!=0);
    return true;
  }
void sendReponse(){
   
    
    if (Serial3.available()>0){
     
      String msgIn=Serial3.readString();
      if (msg.startWith("+IPD,"){
        int endLength=msg.indexOf(":")
        if (endLength>0){
          int length=msgIn.substring(8,endLength).toInt();
          String msgOut=msgIn.subString(endLength,endLength+length);
           Serial.print("received ::::");
          Serial.print(msgOut);
          Serial.println("::::end received :");
        }
        
      }
