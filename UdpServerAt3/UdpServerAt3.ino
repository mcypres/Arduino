





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
  Serial3.println("ATE0"); //no echo
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
  
  Serial.println("send  AT+CIPMUX=1");
  Serial3.println("AT+CIPMUX=1");
  sendReponse();//Response：OK
  
  //Create a UDP transmission, for example, id is 4.
  
  Serial.println("send  AT+CIPSTART=4,\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT+",0");
  Serial3.println("AT+CIPSTART=4,\"UDP\",\""+(String)HOST_NAME+"\","+HOST_PORT+","+REM_PORT+",0");
  
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
 
 // init relais
 
 relaiSetup(); 
  Serial.println("setup end");
  
  delay(1000);
  //init variables
  nbErr=0; 
  loopNum=0;
  errTime=millis();
  sendedByte=0;
  }
  /*******************************************************************/
  void loop() {
  /*******************************************************************/
   /* Send data
  AT+CIPSEND=4,5 // Send 5 bytes to transmission NO.4
  Response > DGFYQ // enter the data, no CR
  SEND OK
  Note: If the number of bytes sent is bigger than the size defined (n), will reply busy, and after sending n number of bytes, reply SEND OK.*/
    //read data from wifi
    readMsg();
    //relaisRefresh();

    //send data
    if(currentTime+DELAY<millis()){
        loopNum++;
        currentTime=millis();
     
        String msg="$$$"+String(loopNum)+" at " +String(millis())+"$$$";
/*---------------------------------------------------------------*/
        Serial3.println("AT+CIPSEND=4,"+String(msg.length()));
        //Serial.println("AT+CIPSEND=4,"+String(msg.length()));
        
        if(readAnswer(">")==true){
           // Serial.println("send msg " +msg);
            Serial3.println(msg);
            if(readAnswer("OK")==false){
              //nbErr++;
              //Serial.println("Message sended no OK");
            }
            else{
              sendedByte+=msg.length();
            }
        }
        else{
            nbErr++;
            Serial.println("AT+CIPSEND=4 sended no >");
        }
 /*-------------------------------------------------------------*/       
    }
    if(errTime+TIME_ERR<millis()){
      float flux =(sendedByte*1000)/(millis()-errTime);
      Serial.println("++++++++++++++++++++++++++++++++++"+String(nbErr)+"erreur sur "+String(loopNum) +"soit "+String(nbErr*100/loopNum) +"% sended " +String(flux)+" bytes/s");
      nbErr=0; 
      loopNum=0;
      errTime=millis();
      sendedByte=0;
    }
    
  }
 
  //Receive data:
  //+IPD,4,n:xxxxxxxxxx // received n bytes, data=xxxxxxxxxxx
  //Delete transmission NO.4
  //AT+CIPCLOSE=4
  //Response：4,CLOSED OK
 // }
 /*******************************************************************/ 
  void relaiSetup(){
      for(int  i=0;i<RELAIS_SIZE;i++){
      pinMode(relais[i], OUTPUT);
      timeout[i]=0;
      digitalWrite(relais[i], HIGH);
 }
  }
 /*******************************************************************/
   void sendReponse(){
   
    
    if (Serial3.available()>0){
      Serial.print("received :");
    Serial.println(Serial3.readString());
    }
  }
/*******************************************************************/
void readMsg(){
   
    
    if (Serial3.available()>0){
     
      String msgIn;
 
      if (Serial3.find((char *)"+IPD,")){
         msgIn=Serial3.readString();
       /*  Serial.print("msgIn:");
         Serial.println(msgIn);*/
        int endLength=msgIn.indexOf(":");
        // Serial.print("endLength: ");
        // Serial.println(endLength);
 
        if (endLength>0){
            int msgLength=msgIn.substring(2,endLength).toInt();
        //   Serial.print("msgIn.substring:");
       //   Serial.println(msgIn.substring(1,endLength)); 
      //    Serial.print("length: ");
     //     Serial.println(msgLength);

            String msgOut=msgIn.substring(endLength,endLength+msgLength+1);
        //  Serial.print("received ::::");
        //  Serial.print(msgOut);
        //  Serial.println("::::end received :");
          writeRelais( msgOut);
        }
        
      }
    }
}
/*******************************************************************/
void writeRelais(String msg){
  
  int numRelais=0;
  Serial.print("writeRelais(const char msg[]:");
  Serial.println(msg);
  
  for(int i=0;i<msg.length() && numRelais<RELAIS_SIZE;i++){
  /*  Serial.print(" msg[");
    Serial.print(i);
    Serial.print("]=");
    Serial.println(msg.charAt(i));*/
    if(msg.charAt(i)=='0'){
      Serial.print("Relais[");
      Serial.print(numRelais);
      Serial.println("]=0");
      
      digitalWrite(relais[numRelais],HIGH);
      numRelais++;
    }
   if(msg.charAt(i)=='1'){
      Serial.print("Relais[");
      Serial.print(numRelais);
      Serial.println("]=1");
      digitalWrite(relais[numRelais],LOW);
      timeout[numRelais]=millis()+TIME_OUT;
      numRelais++;
    }

  }
 
} 
/*******************************************************************/
void relaisRefresh(){
   for(int  i=0;i<RELAIS_SIZE;i++){
      if (timeout[i]<millis()&&timeout[i]!=0){
        digitalWrite(relais[i], HIGH);
        Serial.print("at : ");
        Serial.print(millis());
        Serial.print("  relais ");
        Serial.print(relais[i]);
        Serial.println(" LOW");
        timeout[i]=0;
      }
   } 
}
/*******************************************************************/
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
         Serial.print("nbTry= "+ String(nbTry) +" waited :" +String(millis()-currentTime));
         Serial.print("            *******   i= "+String(i)+ "c= " +c+" HEX ");
         Serial.println((int)c,HEX);
         delay(1);
         if(clef[i]==c){
            i++;
          }
      }
    }while(clef[i]!=0);
    return true;
  }

