/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#define INPUTPIN 4
#define OUTPUPIN 5

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(OUTPUPIN, OUTPUT);
  digitalWrite(OUTPUPIN, HIGH);
  pinMode(INPUTPIN, INPUT);
}

// the loop function runs over and over again forever
void loop() {
    if (Serial.available()>0) {
      int inByte = Serial.read();
      int inByte2;
      if(inByte=='0'){
         digitalWrite(OUTPUPIN, LOW);
        // digitalWrite(13,digitalRead(INPUTPIN));
      }
      if(inByte=='1'){
         digitalWrite(OUTPUPIN, HIGH);
         
      }
    }
    digitalWrite(13,digitalRead(INPUTPIN));
}
