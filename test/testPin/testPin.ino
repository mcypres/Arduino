
#define PIN_SDA 4
#define PIN_SCL 5

// the setup function runs once when you press reset or power the board
boolean sda=true,scl=false;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PIN_SDA, OUTPUT);
  pinMode(5, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  sda=!sda;
  digitalWrite(PIN_SDA, sda);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);                       // wait for a second
  scl=!scl;
  digitalWrite(PIN_SCL, scl);    // turn the LED off by making the voltage LOW
 // delay(1000);                       // wait for a second
}
