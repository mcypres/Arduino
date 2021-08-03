#include <QMC5883L.h>



#include <Wire.h>
#include "tca.h"

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000
#define TCAADDR         0x70

QMC5883L compass1;
QMC5883L compass2;
/*
void tcaSelect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}*/
//int16_t bx1,by1,bz1,bt1;
//int16_t bx2,by2,bz2,bt2;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Tca::tcaselect(0);  
  compass1.init();
  compass1.reconfig();
  compass1.setRange(8);
  delay(100);
  
  Tca::tcaselect(1);
  delay(100);  
  compass2.init();
  compass2.reconfig();
  compass2.setRange(8);
  delay(100);
}

void loop() {
 int r;
int16_t bx1,by1,bz1,bt1;
int16_t bx2,by2,bz2,bt2;

Tca::tcaselect((uint8_t)0);
bx1=0;
by1=0;
bz1=0;
delay(1000);
r = compass1.readRaw(&bx1,&by1,&bz1,&bt1);
if(r==0)Serial.println("*******compass1 invalid read*****");
Serial.print("x1= ");
Serial.println(bx1);
Serial.print("y1= ");
Serial.println(by1);
Serial.print("z1= ");
Serial.println(bz1);
Serial.print("Heading1= ");
Serial.println( getHeadingDegrees( bx1,by1));
delay(500);

Tca::tcaselect((uint8_t)1);
delay(500);
bx2=0;
by2=0;
bz2=0;
r = compass2.readRaw(&bx2,&by2,&bz2,&bt2);
if(r==0)Serial.println("*******compass invalid read*****");

Serial.print("x2= ");
Serial.println(bx2);
Serial.print("y2= ");
Serial.println(by2);
Serial.print("z2= ");
Serial.println(bz2);
Serial.print("Heading2= ");
Serial.println( getHeadingDegrees( bx2,by2));

Serial.print("x= ");
Serial.println(bx1-bx2);
Serial.print("y= ");
Serial.println(by1-by2);
Serial.print("z= ");
Serial.println(bz1-bz2);
Serial.print("Heading= ");
Serial.println( getHeadingDegrees( bx1-bx2,by1-by2));

}

 float getHeadingDegrees(int x,int y){
   float mHeading=atan2(x, y) ;
  //Serial.println("mHeading: "+String(mHeading));
  // Correct for when signs are reversed.
  if (mHeading < 0)
    mHeading += 2 * PI;
  // Check for wrap due to addition of declination.
  if (mHeading > 2 * PI)
        mHeading -= 2 * PI;
  // Convert radians to degrees for readability.
    return  (mHeading * 180 / M_PI);
    
}
