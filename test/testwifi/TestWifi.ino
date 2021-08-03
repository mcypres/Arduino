#include "ESP8266.h"

#define SSID        "wifitelsur_MWAC"
#define PASSWORD    "0019C70D4077X"
//#define SSID        "marc-Aspire-E5-521G"
//#define PASSWORD    "mazel21"
#define HOST_NAME   "192.168.4.2"
#define HOST_PORT   (5416)
void setup() {
  Serial.println("start");
  ESP8266 wifi(Serial,115200);
     if (wifi.setOprToStationSoftAP()) {
        Serial.print("to station + softap ok\r\n");
    } else {
        Serial.print("to station + softap err\r\n");
    }
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");
        Serial.println(wifi.getLocalIP().c_str());       
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    if (wifi.disableMUX()) {
        Serial.print("single ok\r\n");
    } else {
        Serial.print("multi err\r\n");
    }
    
    Serial.print("setup end\r\n");
  String msg="test";
   uint8_t buffer[200] = {0};
 //static uint8_t mux_id = 0;
 
     if (!wifi.registerUDP(HOST_NAME, HOST_PORT)) {
        Serial.println("register udp  err");
    }
    msg.toCharArray((char*)buffer,msg.length() +1);
    char *hello = (char*)&msg;
    Serial.println((char *)buffer);
    wifi.send((const uint8_t*)buffer, msg.length());
   Serial.println("end"); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
void sendMsg(String msg){
 uint8_t buffer[200] = {0};
 //static uint8_t mux_id = 0;
 
 /*    if (!wifi.registerUDP(HOST_NAME, HOST_PORT)) {
        Serial.println("register udp  err");
    }
    msg.toCharArray((char*)buffer,msg.length() +1);
    char *hello = (char*)&msg;
    Serial.println((char *)buffer);
    wifi.send((const uint8_t*)buffer, msg.length());*/
  /*  if (!wifi.unregisterUDP()) {
        Serial.print("unregister udp err\r\n");
    }*/
  /*  
  if (wifi.unregisterUDP(mux_id)) {
        Serial.print("unregister udp ");
        Serial.print(mux_id);
        Serial.println(" ok");
    } else {
        Serial.print("unregister udp ");
        Serial.print(mux_id);
        Serial.println(" err");
    }
    
    mux_id++;
    if (mux_id >= 5) {
        mux_id = 0;
    }*/
}
