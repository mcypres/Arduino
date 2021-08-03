
#include <Logger.h>
HardwareSerial *ptLogOutputSer;
HardwareSerial *ptOutputSer;

Logger* myLogger;
void setup() {
  ptLogOutputSer=&Serial1;
  ptOutputSer= &Serial;
  ptOutputSer->begin(115200);
  ptOutputSer->println("------test Serial-----");
  ptLogOutputSer->begin(74880);
  myLogger= new Logger(ptLogOutputSer,"machineName",E);
  ptLogOutputSer->println("*****************************");
  ptLogOutputSer->println("*****************************");
  _LOG_PRINT(V,"end ","setup");
}

void loop() {
  
  
  ptLogOutputSer->print("*****************************");
  ptLogOutputSer->print(millis());
  ptLogOutputSer->println("\n*****************************");
  _LOG_PRINT(M,"sendMsg",millis());
  delay(500);
 
}
