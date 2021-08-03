
#include "Udpservices.h"
#include "DigitalOutput.h"

#define LOG_BAUDRATE 115200
#define ESP_BAUDRATE 115200
#define ESP_SERIAL Serial3
#define LOG_SERIAL Serial
#define DELAY 1000

DigitalOutput * mDigit;
Udpservices* mUdp;

long currentTime=0;
  long loopNum=0;
  long nbErr=0;
  long errTime;
  long sendedByte;
int relais[]={46,47,48,49,50,51,52,53};
 
void setup() {
  
   ESP_SERIAL.begin(ESP_BAUDRATE);
   LOG_SERIAL.begin(LOG_BAUDRATE);
   LOG_SERIAL.println("setup Start");
   mDigit=new DigitalOutput(8,relais);
    mUdp=new Udpservices(&ESP_SERIAL,&LOG_SERIAL);
    mUdp->init();
    delay(1000);
}

void loop() {
  String msg=mUdp->read();
  if(msg!=""){
    LOG_SERIAL.println(msg);
    mDigit->write(msg);
  }
   if(currentTime+DELAY<millis()){
        loopNum++;
        currentTime=millis();
     
        String msgToSend="$$$"+String(loopNum)+" at " +String(millis())+"$$$";
        LOG_SERIAL.println("msgToSend :"+msgToSend);
        mUdp->send(msgToSend);
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
