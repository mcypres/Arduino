
#include "SensorAnalogGroupInput.h"
#define LOG_SERIAL Serial
#include "ut61e.h"

 #define ADC_TESTED 0 
  
ut61e myMultimetrer(&Serial3);
SensorAnalogGroupInput * mInput;
SensorAnalogGroupInput * mInput2;
//boolean first=true;
#define ECART_LIST 10
  static float ecartList[ECART_LIST];
  int ecartListPos=0;

  /***************************************************************/
void setup() {
  const int pinNumber[]={8,9,10,11};
  const int pinNumber2[]={12,13,14,15};
  const float scale[]={66,66,66,66};
 // const float scale2[]={0.1273,0.1723,0.1723,0.1723};
  const float scale2[]={0.653,1.2995,1.2995,1.2995};
  const int offset[]={512,512,512,512};
  const int offset2[]={0,0,0,0};
  const float refVoltage=5000; // in mV
  const float refVoltage2=20855; // in mV
  // const int refSource=DEFAULT;
  mInput=new SensorAnalogGroupInput('A',INT,pinNumber,scale,offset,refVoltage,DEFAULT);
  mInput2=new SensorAnalogGroupInput('A',VOL,pinNumber2,scale2,offset2,refVoltage2,EXTERNAL);
 
  for(int i=0;i<ECART_LIST;i++){
    ecartList[i]=0;
  }

  Serial.begin(115800);

}
/*****************************************************************************************************/
void loop() {
    
  myMultimetrer.getData();
  myMultimetrer.decode();
  float multValue=myMultimetrer.getValue()*1000;
  Serial.print("\tmultValue ");
  Serial.print(multValue);

  float myvalue=mInput2->getEntryValue(ADC_TESTED);
  int lastRead=mInput2->getLastRead(ADC_TESTED);
  Serial.print("\tlastRead ");
  Serial.print(lastRead);
  Serial.print("\tfullScale ");
  Serial.print((multValue*1023)/lastRead);
  Serial.print("\tmyvalue ");
  Serial.print(myvalue);
 /* Serial.print("\t ecartListPos ");
  Serial.println(ecartListPos);*/
  ecartList[ecartListPos]=multValue-myvalue;
  ecartListPos++;
  ecartListPos%=ECART_LIST;
 // ecartListPos=(ecartListPos++) %ECART_LIST;
 /* Serial.print("\t ecartListPos ");
  Serial.println(ecartListPos);*/

  float moyenne=0;
  for(int i=0;i<ECART_LIST;i++){
    moyenne+= ecartList[i];
  }
  moyenne=moyenne/ECART_LIST;
  Serial.print("\tmoyenne ");
  Serial.print(moyenne);
  float ecartType=0;
   for(int i=0;i<ECART_LIST;i++){
    ecartType+=(moyenne- ecartList[i])*(moyenne- ecartList[i]);
 /*      Serial.print("\tecartList[ ");
    Serial.print(i);
    Serial.print("] ");
    Serial.println(ecartList[i]);*/
 /*   Serial.print("\tmoyenne- ecartList[");
    Serial.print(i);
    Serial.print("] ");
    Serial.println(moyenne- ecartList[i]);
    Serial.print("\tecartType[ ");
    Serial.print(i);
    Serial.print("] ");
    Serial.println(ecartType);*/
  }
  /*Serial.print("\tecartType0 ");
  Serial.println(ecartType);*/
  ecartType=sqrt(ecartType/ECART_LIST);
  
  Serial.print("\tecartType ");
  Serial.println(ecartType);
  delay(500);
 // first=false;
 // }
}
