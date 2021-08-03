void setup() { 
 // #define BAUD_RATE 4800
#define BAUD_RATE 115200
//#define BAUD_RATE 19200
 //#define BAUD_RATE 5000000
Serial.begin (BAUD_RATE);
//Serial1.begin (BAUD_RATE);
//Serial2.begin (BAUD_RATE);
//Serial3.begin (BAUD_RATE);
}
void loop() {
  Serial.print("U"); 
  //Serial2.print("U");
  //Serial3.print("U");
  // put your main code here, to run repeatedly:
delay(500);
}
