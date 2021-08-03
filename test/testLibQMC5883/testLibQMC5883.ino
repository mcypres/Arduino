#include <QMC5883L.h>
#include <Wire.h>

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000

QMC5883L compass;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
compass.init();
compass.reconfig();
compass.setRange(8);
;

}

void loop() {
 int r;
int16_t x,y,z,t;
x=0;
y=0;
z=0;
r = compass.readRaw(&x,&y,&z,&t);
Serial.print("x= ");
Serial.println(x);
Serial.print("y= ");
Serial.println(y);
Serial.print("z= ");
Serial.println(z);
delay(1000);
}
