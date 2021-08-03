#define PIN_FREQ A2
#define PIN_LARGEUR A1
#define PIN_OUT 13
#define TIME_INTERVAL_INPUT 1500
#define MAXTIME 1000
int periodeInput;
int dureeInput;
unsigned long lastTimeInput = 0;
unsigned long lastTimeInput2 = 0;
unsigned long timeLow = 0;
unsigned long timeHight = 0;
unsigned long periode =1;
float tau = 0;
boolean state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIN_OUT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (lastTimeInput  < millis()) {
    periodeInput = analogRead(PIN_FREQ);
    dureeInput = analogRead(PIN_LARGEUR);
    lastTimeInput = millis() + TIME_INTERVAL_INPUT;
    periode = map(periodeInput , 0, 1023, 0, MAXTIME);
    //tau = map((dureeInput), 0, 1023, 0, 1);
    tau=(float)dureeInput/1023;
    timeLow=(unsigned long)((1-tau)*periode);    
    timeHight=(unsigned long)((tau)*periode);
 /*   
    Serial.print("\n\nperiodeInput : ");
    Serial.println(periodeInput);
    Serial.print("dureeInput : ");
    Serial.println(dureeInput);
    Serial.print("\n\periode : ");
    Serial.println(periode);
    Serial.print("tau : ");
    Serial.println(tau);
    Serial.print("\ntimeLow : ");
    Serial.println(timeLow);
    Serial.print("timeHight : ");
    Serial.println(timeHight);*/
  }

  if (lastTimeInput2  < millis()) {
    digitalWrite(PIN_OUT, state);
    //Serial.print("state ");
    
   // Serial.println(state);
    if (state==true) {
      // Serial.println("Hight");
      lastTimeInput2 = millis() + timeLow;
      state = false;
    }
    else {
     //  Serial.println("low");
      lastTimeInput2 = millis() + timeHight;
      state = true;
    }

  }
}
