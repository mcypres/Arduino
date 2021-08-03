

#include "Logger.h"
#define LOG_BAUDRATE 115200

char    machineName[] = "YachtMain";
Logger *myLogger;
HardwareSerial *ptUSB;
void setup() {
 ptUSB = &Serial;
 ptUSB->begin(LOG_BAUDRATE); //USB
  myLogger = new Logger(ptUSB, machineName, E);
  _LOG_PRINT(M, F("setup"), F("Test M"));


  _LOG_PRINT(W, F("setup"), F("Test W"));

  _LOG_PRINT(E, F("setup"), F("Test  E"));

  _LOG_PRINT(D, F("setup"), F("Test D"));
 _LOG_PRINT(V, F("setup"), F("Test V"));

 
 
  
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
