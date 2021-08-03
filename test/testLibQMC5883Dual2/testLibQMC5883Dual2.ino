#include <QMC5883L.h>
#include <Wire.h>
#include "tca.h"

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000
#define TCAADDR         0x70

QMC5883L compass1;
//QMC5883L compass2;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Tca::tcaselect(0);  
  compass1.init();
  compass1.reconfig();
  compass1.setRange(8);
  delay(100);
  
  Tca::tcaselect(0);
  delay(100);  
  compass1.init();
  compass1.reconfig();
  compass1.setRange(8);

}

void loop() {
 int r;
int16_t x1,y1,z1,t1;
int16_t x2,y2,z2,t2;

Tca::tcaselect((uint8_t)0);
x1=0;
y1=0;
z1=0;
r = compass1.readRaw(&x1,&y1,&z1,&t1);
Serial.print("x1= ");
Serial.println(x1);
Serial.print("y1= ");
Serial.println(y1);
Serial.print("z1= ");
Serial.println(z1);
delay(100);

Tca::tcaselect((uint8_t)0);
delay(100);
x2=0;
y2=0;
z2=0;
r = compass1.readRaw(&x2,&y2,&z2,&t2);
Serial.print("x2= ");
Serial.println(x2);
Serial.print("y2= ");
Serial.println(y2);
Serial.print("z2= ");
Serial.println(z2);

Serial.print("x= ");
Serial.println(x1-x2);
Serial.print("y= ");
Serial.println(y1-y2);
Serial.print("z= ");
Serial.println(z1-z2);
Serial.print("Heading= ");
Serial.println( getHeadingDegrees( x1-x2,y1-y2));
delay(1000);
}

 float getHeadingDegrees(int x,int y){
   float mHeading=atan2(x, y) ;
  Serial.println("mHeading: "+String(mHeading));
  // Correct for when signs are reversed.
  if (mHeading < 0)
    mHeading += 2 * PI;
  // Check for wrap due to addition of declination.
  if (mHeading > 2 * PI)
        mHeading -= 2 * PI;
  // Convert radians to degrees for readability.
    return  (mHeading * 180 / M_PI);
}
