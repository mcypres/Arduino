void setup() {
 
Serial.begin(115300);
Serial.print(eepromParamtoString());

}

void loop() {
  // put your main code here, to run repeatedly:

}
char* eepromParamtoString(){
float xOffset=1.22;
float yOffset=-1.22; 
float zOffset=111.2; 
float speedCoef=1200;  
#define PARAM_LIST_MSG " xOffset= 0000,00    yOffset= 0000,00   zOffset= 0000,00   speedCoef= 00000"
//**********************012345678901234567890123456789012345678901234567890123456789012345678901234567890
#define X_OFFSET 10
#define Y_OFFSET 30
#define Z_OFFSET 49
#define SPEED_COEF_OFFSET 70

  static char msg[]=PARAM_LIST_MSG;
 
 char tmp[10]; 
  dtostrf(xOffset,7,2,tmp);
  
  strncpy(msg+X_OFFSET,tmp,7);   
  dtostrf(yOffset,7,2,tmp);
  strncpy(msg+Y_OFFSET,tmp,7);   
  dtostrf(zOffset,7, 2,tmp); 
  strncpy(msg+Z_OFFSET,tmp,7); 
  dtostrf(speedCoef,5, 1,tmp);
  strncpy(msg+SPEED_COEF_OFFSET,tmp,5);   
  return msg;
} 
