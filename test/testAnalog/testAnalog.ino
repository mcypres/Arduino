 int analogPin = 10;
 int val = 0;
 int valMax = 0;
 int valMin = 0;
void setup() {
 
  // put your setup code here, to run once:
  analogReference(DEFAULT);
  //
  //analogReference(INTERNAL1V1);
  ///analogReference(INTERNAL);
  //analogReference(INTERNAL2V56);
  //analogReference(EXTERNAL);
   val = analogRead(analogPin);    // read the input pin
    valMin =2000;    // read the input pin
  Serial.begin(115200);
}

void loop() {
   val = analogRead(analogPin);    // read the input pin
  if(val>valMax) valMax=val;
  if(val<valMin) valMin=val;
  /*Serial.print("\t valMax: ");
  Serial.print(valMax);
  Serial.print("\t valMin: ");
  Serial.print(valMin);  
  Serial.print("\t val: ");*/
  Serial.println(val);
 // delay(1000);
 // for(int i=0;i<100;i++){
    
  //}
  // put your main code here, to run repeatedly:

}
