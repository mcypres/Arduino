#define LOG_SERIAL Serial
#include "ut61e.h"
ut61e myMultimetrer(&Serial3);
void setup() {
  Serial.begin(115200);

}

void loop() {
  
  myMultimetrer.getData();
  myMultimetrer.decode();
  Serial.print("echelle\t");
  Serial.println(myMultimetrer.getEchelle());
  Serial.print("unit\t");
  Serial.println(myMultimetrer.getUnit());
  Serial.print("value\t");
  Serial.println(myMultimetrer.getValue());
  Serial.print("getSelection\t");
  Serial.println(myMultimetrer.getSelection());
  delay(300);
}
