
#include "SensorAnalogGroupInput.h"

SensorAnalogGroupInput * mInput;
SensorAnalogGroupInput * mInput2;
//boolean first=true;
void setup() {
   const int pinNumber[]={8,9,10,11};
   const int pinNumber2[]={12,13,14,15};
   const float scale[]={66,66,66,66};
   const float scale2[]={0.1273,0.1723,0.1723,0.1723};
   const int offset[]={512,512,512,512};
   const int offset2[]={0,0,0,0};
   const float refVoltage=5000; // in mV
   const float refVoltage2=2087; // in mV
   
  // const int refSource=DEFAULT;
  mInput=new SensorAnalogGroupInput('A',INT,pinNumber,scale,offset,refVoltage,DEFAULT);
  mInput2=new SensorAnalogGroupInput('A',VOL,pinNumber2,scale2,offset2,refVoltage2,EXTERNAL);
 
  

  Serial.begin(115800);

}

void loop() {
 // if (first==true){
  Serial.println(mInput->toNmea());
  Serial.println(mInput->toNmea());
  Serial.println(mInput->toNmea());
  Serial.println(mInput2->toNmea());
  Serial.println(mInput2->toNmea());
  Serial.println(mInput2->toNmea());
   Serial.println(mInput2->toNmea());
  Serial.println(mInput2->toNmea());
  Serial.println(mInput2->toNmea());
  Serial.println("*******************");
  delay(5000);
 // first=false;
 // }
}
