long currentTime=millis();
long flashTime=millis();
int number;
boolean state=false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   pinMode(13, OUTPUT);
   
}

void loop() {
  if (currentTime+1000<millis()){
    Serial.println("hello");
    currentTime=millis();
  }
  if (Serial.available()>0){
     int n=Serial.parseInt();
     if (n>0){
      number=n;
     }
     Serial.print("input :");
     Serial.println(number);
     state=true;
     digitalWrite(13, state);
  }
  if ((number>0)&&(flashTime+500<millis())){
    Serial.print("number: ");
    Serial.println(number);
      flashTime=millis();
      //state=!state;
      if(state==true){
      digitalWrite(13, HIGH);
      state=false;
      }
      else{
        digitalWrite(13, LOW);
        state=true;
        number--;
      }
      
  }

}
void mblink13(){
   digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);  
}

