#include <EEPROM.h>
//int xVal = 1234; //the int to write to EEPROM
long lastTime=0;
#define TIME_INTERVAL 1000

struct IniField{
  float x;
  float y;
  float z;
};
union EepromBuff{
  IniField ini;
  byte byteBuffer[sizeof(IniField)];
  float vect[3];
} eepromBuff;


void setup() {
  Serial.begin(115200);
  EEPROM.begin(4);
}
/*
  EEPROM.write(0, highByte(xVal)); //write the first half
  EEPROM.write(1, lowByte(xVal)); //write the second half

  Serial.println(xVal);
}*/
/*
void setup() {
  Serial.begin(115200);

  byte high = EEPROM.read(0); //read the first half
  byte low = EEPROM.read(1); //read the second half
  
  int xVal = (high << 8) + low; //reconstruct the integer
  Serial.println(xVal);
*/
  /*
   *  In this case, '<<' is a bit shift operator, it moves
   *  the eight high bytes to the top of the int and then the
   *  statement adds the low side of the int to the end. This
   *  ensures that the int comes back in correct order.
   */
//}

void loop() {
  if(lastTime+TIME_INTERVAL <millis()){
    byte mByte= EEPROM.read(0);
    Serial.print("mByte ");
    Serial.println(mByte,HEX);
    Serial.println((char)EEPROM.read(0));
    lastTime=millis();
  }
  if(Serial.available() > 0 ) {
    char received = Serial.read();
    if (received != 0xD && received != 0xA) {
    Serial.print("receved ");
      Serial.println(received,HEX);
      EEPROM.write(0,(byte)received);
      EEPROM.commit();
    }
 /*     *inData += received;
      if (received == '\n') {
        Serial.println(*inData);
      }*/
  }

}
