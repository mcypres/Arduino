#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <SensorQMC5883.h>
#define TCAADDR 0x70
#define NB_SENSOR 2
/* Assign a unique ID to this sensor at the same time */
/*
Adafruit_HMC5883_Unified mag1 = Adafruit_HMC5883_Unified(1);
Adafruit_HMC5883_Unified mag2 = Adafruit_HMC5883_Unified(2);
*/
SensorQMC5883 mag[NB_SENSOR];

void displaySensorDetails(Adafruit_HMC5883_Unified *mag)
{
  sensor_t sensor;
  mag->getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.begin(5,4);
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


void setup(void) 
{
  Serial.begin(115200);
  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
 for(int i=0;i<NB_SENSOR;i++){
     tcaselect(i);
      Serial.print("port [");
      Serial.print(i);
      Serial.println("] :");
     /* Initialise the 1st sensor */
     mag[i] =  SensorQMC5883(i);
    if(!mag[i].begin())
    {
      /* There was a problem detecting the HMC5883 ... check your connections */
     
      Serial.println(" no HMC5883 detected ");
    }
    else{
       /* Display some basic information on this sensor */
      displaySensorDetails(&mag[i]);
    }
 }
}

void loop(void) 
{
  Serial.print("loop start");
  for(int i=0;i<NB_SENSOR;i++){
    Serial.print("loop ");
    Serial.println(i);
    /* Get a new sensor event */ 
    sensors_event_t event; 
    
    tcaselect(i);
    Serial.print("avant get event ");
    mag[i].getEvent(&event);
     Serial.print("apres get event ");
   
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("Sensor ");
    Serial.println(i);
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
 
    delay(500);
  }
}
