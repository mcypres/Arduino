#include <EEPROM.h>
//int xVal = 1234; //the int to write to EEPROM
long lastTime=0;
#define TIME_INTERVAL 10000
#define ENTETE_COMMANDE "$"
#define LG_ENTETE_COMMANDE 1
#define SEP ","

String inData;
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

/******************************************/
void setup() {
  
  Serial.begin(115200);
  Serial.println("setup start");
  EEPROM.begin(sizeof(eepromBuff));
 /* for(int i=0;i< sizeof(eepromBuff);i++){
    
  }*/
  Serial.println(toString());
  Serial.println("setup end");
}


/********************************************/
void loop() {
  if(lastTime+TIME_INTERVAL <millis()){
   // Serial.println(toString());
    /*
    byte mByte= EEPROM.read(0);

    Serial.print("mByte ");
    Serial.println(mByte,HEX);
    Serial.println((char)EEPROM.read(0));
    lastTime=millis();*/
  }
  if(Serial.available() > 0 ) {
    char received = Serial.read();
    Serial.print(received);
    //if (received != 0xD && received != 0xA) {
    if (received != -1) {
      inData += received;
      if (received == '\n') {
        Serial.print("\n received :");
        Serial.println(inData);
        int n=inData.indexOf(ENTETE_COMMANDE);
        if (n>=0){
          n +=LG_ENTETE_COMMANDE;
          for(int i=0; i<3;i++){
            int j= inData.indexOf(SEP,n);
            if(j<0){
              j=inData.length();
            } 
            Serial.print("inData.substring");
            Serial.print(n);
            Serial.print(",");
            Serial.println(j);
            Serial.println(" = ");
            Serial.println(inData.substring(n,j));          
            eepromBuff.vect[i]=inData.substring(n,j).toFloat();
            n=j+1;
          }
          writeEewriprom( &eepromBuff);
          Serial.println(toString());
        }
        inData="";
      }
    }
  }
}

/**********************************************/
void readEeprom(EepromBuff* eepromBuff){
  Serial.print("readEewriprom ");
  Serial.println(toString(eepromBuff));
  for (int i=0; i< sizeof(*eepromBuff);i++){
    eepromBuff->byteBuffer[i]=EEPROM.read(i);
    Serial.print("\t");
    Serial.print(eepromBuff->byteBuffer[i],HEX);
    if ((i% 10)==0)Serial.println();
  }
}
/*****************************************************/
void writeEewriprom(EepromBuff* eepromBuff){
  Serial.print("writeEewriprom ");
  Serial.println(toString(eepromBuff));
   for (int i=0; i< sizeof(*eepromBuff);i++){
    Serial.print("\t");
    Serial.print(eepromBuff->byteBuffer[i],HEX);
    if ((i% 10)==0)Serial.println();
    EEPROM.write(i,eepromBuff->byteBuffer[i]);
  }
  EEPROM.commit();
}
/**************************************************/
String toString( EepromBuff* in){
  String rep="";
  rep +="\n x= " +String(in->ini.x);
  rep +="\t y= " +String(in->ini.y);
  rep +="\t z= "+String(in->ini.z);
  return rep;
}
/**************************************************/
String toString(){
  EepromBuff in;
  readEeprom( &in);
  Serial.println(toString(&in));
  return toString(&in);
}
