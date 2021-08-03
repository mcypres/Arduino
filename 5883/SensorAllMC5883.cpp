/*
 * SensorAllMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include "SensorAllMC5883.h"
#include <Adafruit_HMC5883_U.h>

// Nmea message definition
//$--MWV,x.x,a,x.x,a*hh

#define NMEA_MWV_MSG "$PAMWV,000.0,R,000.00,N,A*     "
//********************0123456789012345678901234567890


#define BREARING_OFFSET 7
#define SPEED_OFFSET 15
#define MSG_LENGTH_T_CHECKSUM 24
#define CHECKSUM_OFFSET 26
#define GIRO_TIME_INTERVAL 1000



// anemotetre param definition
//#define ANEMO_INTERUPT 1
//#define ANEMO_PIN 14
#define MAX_VALUE 10000
#define MIN_VALUE -10000

SensorAllMC5883::SensorAllMC5883(HardwareSerial *ptOutputSer,
				Logger *myLogger,
				float declinationAngleDegres,
				float declinationAnglemin,
				int id,
				float xOffset,
				float yOffset,
				float zOffset,
				float speedCoef
				) :Sensor(ptOutputSer,myLogger){
	_LOG_PRINT(M, &"SensorAllMC5883", "Start constructor");
	this->xOffset=xOffset;
	this->yOffset=yOffset;
	this->zOffset=zOffset;
	minx=MAX_VALUE;
	miny=MAX_VALUE;
	minz=MAX_VALUE;
	maxx=MIN_VALUE;
	maxy=MIN_VALUE;
	maxz=MIN_VALUE;
 	GanemoCount=0;
	this->speedCoef=speedCoef;
	declinationAngle = (declinationAngleDegres + (declinationAnglemin / 60)) * M_PI / 180;
	mLastTime=0;
//	sensorName="HMC5883";
	
	

String SensorAllMC5883::toString(sensors_event_t event;){
	String retVal;
	
	getData();
	//mag.getEvent(&event);
	retVal="\n\tX:\t"+(String)event.magnetic.x;
  	retVal+="\tY:\t"+(String)event.magnetic.y;
  	retVal+="\tZ:\t"+(String)event.magnetic.z+"uT";                                             
  	retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\tcap (°)  :\t"+(String)headingDegrees;
	//retVal+="\n\tvitesse (N)  :\t"+(String)speed;*/
	return retVal;
}

char* SensorAllMC5883::eepromParamtoString(){

#define PARAM_LIST_MSG " xOffset= 0000,00    yOffset= 0000,00   zOffset= 0000,00   speedCoef= 00000"
//**********************012345678901234567890123456789012345678901234567890123456789012345678901234567890
#define X_OFFSET 10
#define Y_OFFSET 30
#define Z_OFFSET 49
#define SPEED_COEF_OFFSET 70

  static char msg[]=PARAM_LIST_MSG;
 
 char tmp[10]; 
  dtostrf(xOffset,7,2,tmp);  
  strncpy(msg+X_OFFSET,tmp,7);
  
  dtostrf(yOffset,7,2,tmp);
  strncpy(msg+Y_OFFSET,tmp,7);
  
  dtostrf(zOffset,7, 2,tmp); 
  strncpy(msg+Z_OFFSET,tmp,7);
  
  dtostrf(speedCoef,5, 1,tmp);
  strncpy(msg+SPEED_COEF_OFFSET,tmp,5);   
  return msg;
} 

char* SensorAllMC5883::toNmea(){	
	return toNmea(getHeadingDegrees(event),getSpeed());
}

char* SensorAllMC5883::toNmea(float HeadingDegrees,float Speed){
/*
MWV	Wind Speed and Angle
       1   2 3   4 
$--MWV,x.x,a,x.x,a*hh

	1 Wind Angle, 0 to 360 degrees
	2 Reference, R = Relative, T = True
	3 Wind Speed
	4 Wind Speed Units, K/M/N
	(5 Status, A = Data Valid)
	5(6) Checksum
VWR  Relative Wind Speed and Angle
       1   2 3   4 5   6 7   8 9
       |   | |   | |   | |   |
$--VWR,x.x,a,x.x,N,x.x,M,x.x,K*hh

	1 Wind direction
 	2 Wind direction
	3 Speed
	4 N = Knots
	5 Speed
	6 M = Meters Per
	7 Speed
	8 K = Kilometers
	9 Checksum
*/
	static char msg[]=NMEA_MWV_MSG;
	char tmp[10];
 	dtostrf(HeadingDegrees,5, 1,tmp);	
	strncpy(msg+BREARING_OFFSET,tmp,5);	
	dtostrf((float) Speed,6,2,tmp);
	strncpy(msg+SPEED_OFFSET,tmp,6);	
 	_LOG_PRINT(V,"msg avant checksum",msg);
	checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);	
	_LOG_PRINT(V,"msg apres checksum",msg);
	return msg;
}


String SensorAllMC5883::getSensorDetail(){
 	sensor_t sensor;
	String retVal;
  	mag.getSensor(&sensor);
  	retVal= "\n\tSensor:\t"+(String)sensor.name;
  	retVal+="\n\tDriver Ver:\t"+(String)sensor.version;
  	retVal+="\n\tUnique ID:\t"+(String)sensor.sensor_id;
  	retVal+="\n\tMax Value:\t"+(String)sensor.max_value+" uT";
 	retVal+="\n\tMin Value:\t"+(String)sensor.min_value+" uT";
  	retVal+="\n\tResolution:\t"+(String)sensor.resolution+" uT";
 	return retVal;
}

 float SensorAllMC5883::getHeadingDegrees(sensors_event_t event,
					float declinationAngle,
					float xOffset,
					float yOffset){
	float mHeading=atan2(event.magnetic.y-yOffset, event.magnetic.x-xOffset) + declinationAngle;

	// Correct for when signs are reversed.
	if (mHeading < 0)
		mHeading += 2 * PI;
	// Check for wrap due to addition of declination.
	if (mHeading > 2 * PI)
    		mHeading -= 2 * PI;
	// Convert radians to degrees for readability.
  	return  (mHeading * 180 / M_PI);
}

float SensorAllMC5883::getSpeed(){
	unsigned long myTime=millis();
	float mSpeed=-1;
	if ((myTime-mLastTime)>0){
		_LOG_PRINT(D,"GanemoCount :",(String)GanemoCount);
		mSpeed=((float)GanemoCount*speedCoef)/(float)(myTime-mLastTime);
	}
	GanemoCount=0;
	// update time	
	mLastTime =myTime;
	return mSpeed;
}
sensors_event_t  SensorAllMC5883::getEvent(){
	_LOG_PRINT(V,"SensorAllMC5883 getEvent :","Start");
	mag.getEvent(&event);
	_LOG_PRINT(V,"SensorAllMC5883 getEvent :","end");
	return event;
}

 void SensorAllMC5883::getData(sensors_event_t event;){
	unsigned long myTime=millis();
	magnetudexy = sqrt(sq(event.magnetic.y-yOffset) + sq(event.magnetic.x-xOffset));
  	magnetude = sqrt(sq(event.magnetic.y-yOffset) + sq(event.magnetic.x-xOffset) + sq(event.magnetic.z-zOffset));
	headingDegrees = getHeadingDegrees( event,
					 declinationAngle,
					 xOffset,
					 yOffset);
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
	// get anemo data
	//Serial.println("myTime :"+(String)myTime);
/*	_LOG_PRINT(V,"myTime :",(String)myTime);

	
	if ((myTime-mLastTime)>0){
		_LOG_PRINT(D,"GanemoCount :",(String)GanemoCount);
		speed=(GanemoCount*speedCoef)/(myTime-mLastTime);
	}
	else{
		speed=-1;
	}
	GanemoCount=0;
		// update time	
	mLastTime =myTime;*/
	//speed=getSpeed();
}


SensorAllMC5883::~SensorAllMC5883() {
	// TODO Auto-generated destructor stub
}

