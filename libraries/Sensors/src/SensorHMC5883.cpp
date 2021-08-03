/*
 * SensorHMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include <SensorHMC5883.h>

// anemotetre param definition
#define MAX_VALUE 10000
#define MIN_VALUE -10000

SensorHMC5883::SensorHMC5883(String sensorName,
			Logger* myLogger,
			int declinationAngleDegres=0,
			float declinationAnglemin=0,
			float xOffset=0,
			float yOffset=0,
			float zOffset=0,
			int id=0) :SensorMagneto( sensorName,
						myLogger,
						declinationAngleDegres,
						declinationAnglemin,
						xOffset,
						yOffset,
						zOffset,
						id){


	//Serial.println("HMC5883 start");
	Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(id);
	//Serial.println("HMC5883 created");
	declinationAngle = (declinationAngleDegres + (declinationAnglemin / 60)) * M_PI / 180;
	//lastTime=0;
	sensorName="HMC5883";
	ErrFlag=false;
	if (!mag.begin()) {
		ErrorMsg="Could not find a valid HMC5883 sensor, check wiring!";
		Serial.println(ErrorMsg);
		ErrFlag=true;
		return;
	  }
	 //Serial.println("SensorHMC5883 ok"); 
	this->xOffset=xOffset;
	this->yOffset=yOffset;
	this->zOffset=zOffset;
	mag.setMagGain(HMC5883_MAGGAIN_8_1);
	minx=MAX_VALUE;
	miny=MAX_VALUE;
	minz=MAX_VALUE;
	maxx=MIN_VALUE;
	maxy=MIN_VALUE;
	maxz=MIN_VALUE;
}

String SensorHMC5883::toString(){
	
	String retVal;
	mag.getEvent(&event);

	if(minx>event.magnetic.x){
		minx=event.magnetic.x;
	}
	if(miny>event.magnetic.y){
		miny=event.magnetic.y;
	}
	if(minz>event.magnetic.z){
		minz=event.magnetic.z;
	}
	if(maxx<event.magnetic.x){
		maxx=event.magnetic.x;
	}
	if(maxy<event.magnetic.y){
		maxy=event.magnetic.y;
	}
	if(maxz<event.magnetic.z){
		maxz=event.magnetic.z;
	}


	retVal="\n\tX:\t"+(String)event.magnetic.x;
  	retVal+="\n\tY:\t"+(String)event.magnetic.y;
  	retVal+="\n\tZ:\t"+(String)event.magnetic.z+"uT";
	retVal+="\n\tminx:\t "+(String)minx+"uT";
	retVal+="\n\tminy:\t "+(String)miny+"uT";
	retVal+="\n\tminz:\t "+(String)minz+"uT";
	retVal+="\n\tmaxx:\t "+(String)maxx+"uT";
	retVal+="\n\tmaxy:\t "+(String)maxy+"uT";
	retVal+="\n\tmaxz:\t "+(String)maxz+"uT";
	retVal+="\n\tx offset:\t "+(String)((maxx+minx)/2)+"uT";
	retVal+="\n\ty offset:\t "+(String)((maxy+miny)/2)+"uT";
	retVal+="\n\tz offset:\t "+(String)((maxz+minz)/2)+"uT";



  	retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\n\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\n\tcap (°)  :\t"+(String)headingDegrees;
	//retVal+="\n\tvitesse (N)  :\t"+(String)speed;
	return retVal;
}

 void SensorHMC5883::getData(){
	mag.getEvent(&event);
	_LOG_PRINT(D, "event x :", event.magnetic.x);
	_LOG_PRINT(D, "event y :", event.magnetic.y);

	magnetudexy = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x));
  	magnetude = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x) + sq(event.magnetic.z));
	heading = atan2(event.magnetic.y, event.magnetic.x) + declinationAngle;
	// Correct for when signs are reversed.
	if (heading < 0)
		heading += 2 * PI;
	// Check for wrap due to addition of declination.
	if (heading > 2 * PI)
    		heading -= 2 * PI;
	// Convert radians to degrees for readability.
  	headingDegrees = heading * 180 / M_PI;
 }


SensorHMC5883::~SensorHMC5883() {
	// TODO Auto-generated destructor stub
}

