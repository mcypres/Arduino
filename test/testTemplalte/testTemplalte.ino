
#include  <EepromSrtream.h>
 struct IniField{
  float x;
  float y;
  float z;
};
union EepromBuff{
  IniField ini;
  
  float vect[3];
} ;
  EepromSrtream<EepromBuff> permanentSave;
void setup() {
  EepromBuff * buff= permanentSave.readEeprom();
  Serial.println(permanentSave.toString());
  buff->datas.ini.x+=10;
  buff->datas.ini.y-=10;
  buff->datas.ini.z*=1.1;
  permanentSave.writeEewriprom();
  Serial.println(permanentSave.toString());
}

void loop() {
  // put your main code here, to run repeatedly:

}
