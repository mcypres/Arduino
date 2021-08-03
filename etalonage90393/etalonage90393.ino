#include <Wire.h>

#include "Adafruit_MLX90393.h"
#include "Logger.h"
#include <AsyncrSerial.h>
#include "stdlib.h"

Adafruit_MLX90393 sensor = Adafruit_MLX90393();
Logger *myLogger;
AsyncrSerial*  asyncrLogSer;
AsyncrSerial*  asyncrSer;
HardwareSerial *ptUSB;
static String Msg="";


void setup(void)
{
  Serial.begin(115200);
  ptUSB=&Serial;
  Serial.println("Starting Setup");

  /* Wait for serial on USB platforms. */
  while(!Serial) {
      delay(10);//96
  }
  myLogger= new Logger(&Serial,"Girouette",D);
  //asyncrLogSer=new AsyncrSerial(ptUSB,myLogger);
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
  static float refHeading;
  float heading;
  float heading360;
  float magnitude;
   if(Serial.available()>0){
  
    // get the new byte:
    char inChar = (char)Serial.read();
    //
    Serial.print(inChar);
    // add it to the msgWifiOut:
    Msg+= inChar;
    if (inChar == '\n'&& Msg.length()>0) {
      refHeading=std::atoi(Msg.c_str());
      Msg=""; 
      float x,y,z;    
      if(sensor.readData(&x, &y, &z)) {
        // int heading = 180.0*atan2(y,x)/M_PI;
        heading = (3200.0*atan2(y,x))/M_PI;
        if(heading<=0){
          heading =heading+ 6400;
        }
                heading360 = (360.0*atan2(y,x))/M_PI;
        if(heading360<=0){
          heading360 =heading360+ 360;
        }
        magnitude=sqrt(x*x+y*y+z*z);
        /*Serial.print(refHeading);*/
        Serial.print(" magnitude : ");Serial.print(magnitude,0);
        Serial.print("\t heading360 : ");Serial.println(heading360,0);
        Serial.print(refHeading,0);Serial.print(",");Serial.print(heading,0);Serial.print(",");Serial.print(heading-refHeading,0);
       
        Serial.print(",");Serial.print(x, 0); Serial.print(",");Serial.print(x/500, 1);Serial.print(",");
        Serial.print(y, 0); Serial.print(",");Serial.print(y/500, 1);Serial.print(",");    
        Serial.print(z, 0); Serial.print(",");Serial.println(z/500, 1);//Serial.print(",");
      }
      else {
        Serial.println("Unable to read XYZ data from the sensor.");
      }
    }
   }

    //delay(500);
}
