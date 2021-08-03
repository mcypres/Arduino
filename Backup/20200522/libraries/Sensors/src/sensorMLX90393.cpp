/*
 file SensorMLX90393.cpp
 marc cypres
 11/08/2020
 */

#include <SensorMLX90393.h>

SensorMLX90393::SensorMLX90393(){
	//this->myLogger=myLogger;
	 if (Adafruit_MLX90393::begin()){
	  	
		Serial.println("Found a MLX90393 sensor");
		Adafruit_MLX90393::setGain(MLX90393_GAIN_5X);
		status=true;
	  }
	  else{
		status=false;
	  }
}

int SensorMLX90393::getHeadingDeg(){
//int getHeadingDeg(){
	float headingRad= SensorMLX90393::getHeadingRad();
	int rep=(int)(180*headingRad/M_PI);
	//int rep=(int)(180*(getHeadingRad())/M_PI);

	return rep;
}
float SensorMLX90393::getHeadingRad(){
//float getHeadingRad(){

	float heading=-1,x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
  	//if(readData(&x, &y, &z)) {
       	heading = atan2(y,x);
       	/*Serial.print("heading : ");
    	Serial.println(heading);*/
        if(heading<0){
        	heading =heading+(2*M_PI);
        }
        heading=(2*M_PI)-heading;
     }	
     return heading;	
}
int SensorMLX90393::getHeadingPm(){
	return (int)(3200*getHeadingRad()/M_PI);
}
int SensorMLX90393::getMagnitude(){
	float magnitude=-1,x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
		magnitude=sqrt(x*x+y*y+z*z);
    }
    return int(magnitude);
}	
int SensorMLX90393::getMagnitudeXY(){
	float magnitude=-1,x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
		magnitude=sqrt(x*x+y*y);
    }
    return int(magnitude);	
}
int SensorMLX90393::getBx(){
	float x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
		return int(x);
    }
    return 0;		
}
int SensorMLX90393::getBy(){
	float x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
		return int(y);
    }
    return 0;	
}
int SensorMLX90393::getBz(){
	float x,y,z;
 	if(Adafruit_MLX90393::readData(&x, &y, &z)) {
		return int(z);
    }
    return 0;	
}
bool SensorMLX90393::getStatus(){
	return status;
}

