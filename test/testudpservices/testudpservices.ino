#include <CircBuf.h>

#include <Udpservices.h>
#define ESP_SERIAL Serial3
#define LOGSERIAL Serial
#define TIME_INTERVAL 3000
Udpservices* mUdp;
long lastTime;
void setup() {
  LOGSERIAL.begin(115200);
  ESP_SERIAL.begin(115200);
  // put your setup code here, to run once:
  mUdp=new Udpservices(&ESP_SERIAL,&LOGSERIAL);
  mUdp->init();
  delay(1000);
  lastTime=millis();
}

void loop() {
  String msg;
  
 
  msg=mUdp->readLine();
  if (msg.length()>0){
    Serial.println(msg);
  }
  if (lastTime+TIME_INTERVAL <millis()){
    lastTime=millis();
    mUdp->println("1234567890TEST12345678901234567890");
    delay(100);
    mUdp->println("qwerty");
  }
  
}
