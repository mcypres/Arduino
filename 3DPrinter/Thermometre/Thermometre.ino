/*
  Analog Input
*/

#define VREF 1076 //mV
#define NB_ANAOG_STEP 1023
#define LM36OFFSET_VOLTAGE 750  //mV
#define LM36OFFSET_TEMP 25
#define LM36Coef 10
#define DELAY 1000
#define FAN_PIN A3
#define SENSOR_PIN A0
#define REF_PIN A1
#define SENSI_PIN A2
#define MIN_TEMP 0
#define MAX_TEMP 500
#define MIN_HYST 10
#define MAX_HYST 50


//int sensorPin = A0;    // select the input pin for the potentiometer

int ledPin = 13;      // select the pin for the LED
float sensorValue = 0;  // variable to store the value coming from the sensor
float consigneTemp=10;
float hysterisseTemp=5;
boolean state=false;
int fanSpeed;

void setup() {
  
  // declare the ledPin as an OUTPUT:
  pinMode(FAN_PIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  analogReference(INTERNAL);
  
  Serial.begin(9800); 
  Serial.print("Start");
}

void loop() {
  int outputValue;
  consigneTemp= map(analogRead(REF_PIN),0,1023,MIN_TEMP,MAX_TEMP)/10;
  hysterisseTemp= map(analogRead(SENSI_PIN),0,1023,MIN_HYST,MAX_HYST)/10;
  // read the value from the sensor:
  sensorValue = readTemp();
  
  Serial.print("sensorValue = ");
  Serial.print(sensorValue);
  Serial.print("\t consigneTemp = ");
  Serial.print(consigneTemp);
  Serial.print("\t hysterisseTemp = ");
  Serial.print(hysterisseTemp);
  Serial.print("\t state = ");
  Serial.print(state);
  
  if(sensorValue<=(consigneTemp-hysterisseTemp)){
    Serial.print("\t STOP");
    outputValue=0;
    state=false;
  }
  else{
    if(sensorValue>=consigneTemp){
      Serial.print("\tSTART");
      state=true;
      outputValue=255;
    }
    else{
      outputValue=map(sensorValue,consigneTemp-hysterisseTemp,consigneTemp,0,255);
    }
  }
  Serial.print("\t outputValue = ");
  Serial.println(outputValue);
  analogWrite(FAN_PIN, outputValue);
  // turn the ledPin on
  digitalWrite(ledPin, state);
  
  delay(DELAY);
}

/****************************************************************************/

float readTemp() {  // get temperature and convert it to celsius
  
  long nbSteps= analogRead(SENSOR_PIN);
    Serial.print("nbSteps = ");
  Serial.print(nbSteps);
  float voltage=(VREF*nbSteps)/NB_ANAOG_STEP; // voltage in mV
  Serial.print("\t voltage = ");
  Serial.println(voltage);
  float temp=((voltage-LM36OFFSET_VOLTAGE)/LM36Coef)+LM36OFFSET_TEMP;
  return temp;
}
