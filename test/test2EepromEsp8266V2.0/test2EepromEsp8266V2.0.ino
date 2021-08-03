#include <EEPROM.h>
#include <EepromSrtream.h>
#include <Decoder.h>
//int xVal = 1234; //the int to write to EEPROM
long lastTime=0;
#define TIME_INTERVAL 10000
#define ENTETE_COMMANDE "$"
#define LG_ENTETE_COMMANDE 1
#define SEP ","

EepromSrtream <EepromBuff>myEepromSrtream;
Decoder mydecoder(&myEepromSrtream);
EepromBuff* ptEepromBuff;
String inData="";

/******************************************/
void setup() {
  
  Serial.begin(115200);
  Serial.println("setup start");
 // EEPROM.begin(sizeof(eepromBuff));
 /* for(int i=0;i< sizeof(eepromBuff);i++){
    
  }*/
  ptEepromBuff=myEepromSrtream.readEeprom();
  Serial.println(mydecoder.toString());
  Serial.println("setup end");
}


/********************************************/
void loop() {
  if(lastTime+TIME_INTERVAL <millis()&&Serial.available() > 0){
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
          mydecoder.decodeCommande(inData.substring(n)); 
        }
        inData="";
      }
    }
  }
}

/**********************************************/
/*
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
*/
/*****************************************************/
/*
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
}*/
/**************************************************/

 /*
String toString( EepromBuff* in){
  String rep="";
  rep +="\n x= " +String(in->ini.x);
  rep +="\t y= " +String(in->ini.y);
  rep +="\t z= "+String(in->ini.z);
  return rep;
}
*/
/**************************************************/
/*
String toString(){
  EepromBuff in;
  readEeprom( &in);
  Serial.println(toString(&in));
  return toString(&.in);
}*/
