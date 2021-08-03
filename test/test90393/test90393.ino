#include <Wire.h>

#include "Adafruit_MLX90393.h"

Adafruit_MLX90393 sensor = Adafruit_MLX90393();

void setup(void)
{
  Serial.begin(115200);

  /* Wait for serial on USB platforms. */
  while(!Serial) {
      delay(10);
  }
  

  Serial.println("Starting Adafruit MLX90393 Demo");

  if (sensor.begin())
  {
    Serial.println("Found a MLX90393 sensor");
    sensor.setGain(MLX90393_GAIN_5X);
  }
  else
  {
    Serial.println("No sensor found");
  }

}
void loop(){
  float x,y,z,heading360,magnitude,magnitudeXY;
    if(sensor.readData(&x, &y, &z)) {
        Serial.print("X: "); Serial.print(x, 4); Serial.print(" uT \t");Serial.print(x/500, 1);Serial.println(" % Sat");
        Serial.print("Y: "); Serial.print(y, 4); Serial.print(" uT \t");Serial.print(y/500, 1);Serial.println(" % Sat");
        Serial.print("Z: "); Serial.print(z, 4); Serial.print(" uT \t");Serial.print(z/500, 1);Serial.println(" % Sat");
        Serial.println("\n\n\n");

       // int heading = 180.0*atan2(y,x)/M_PI;
       int heading = 3200.0*atan2(y,x)/M_PI;
        if(heading<0) heading =heading+ 6400;
        Serial.print("angle: "); Serial.println(heading);
        heading360 = (360.0*atan2(y,x))/M_PI;
        if(heading360<0){
          heading360 =heading360+ 360;
        }
      magnitude=sqrt(x*x+y*y+z*z);
      magnitudeXY=sqrt(x*x+y*y);

      /*Serial.print(refHeading);*/
      Serial.print(" magnitude : ");Serial.print(magnitude,0);
      Serial.print(" magnitudeXY : ");Serial.print(magnitudeXY,0);
      Serial.print("\t heading360 : ");Serial.println(heading360,0);
          
    } else {
       // Serial.println("Unable to read XYZ data from the sensor.");
    }


    delay(500);
}
