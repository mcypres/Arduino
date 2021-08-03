/*
 * SensorQMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */

#include "SensorQMC5883.h"
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

SensorQMC5883::SensorQMC5883(HardwareSerial *ptOutputSer,
				Logger *myLogger,
				float declinationAngleDegres,
				float declinationAnglemin,
				int id,
				float xOffset,
				float yOffset,
				float zOffset,
				float speedCoef
				) :Sensor(ptOutputSer,myLogger){
	_LOG_PRINT(M, &"SensorQMC5883", "Start constructor");
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
	sensorName="QMC5883";
	
	_LOG_PRINT(M, &"SensorQMC5883 constructor", "HMC5883 start");
	//Serial.println("HMC5883 start");
	
	//mag = Adafruit_HMC5883_Unified(id);
	init(mag);						
	_LOG_PRINT(M, &"SensorQMC5883 constructor","HMC5883 created");
}
void SensorQMC5883::init(MechaQMC5883 mag ){
	mag.init();
/*	ErrFlag=false;
	if (!mag.begin()) {
		ErrorMsg="Could not find a valid HMC5883 sensor, check wiring!";
		_LOG_PRINT(E," constructor",ErrorMsg);
		ErrFlag=true;
		return;
	}	 
	mag.setMagGain(HMC5883_MAGGAIN_8_1);*/
 	_LOG_PRINT(M," constructor","SensorQMC5883 ok");
}
String SensorQMC5883::toString(){
	String retVal;
	
	getData();
	//mag.getEvent(&event);
	retVal="\n\tX:\t"+(String)event.magnetic.x;
  	retVal+="\tY:\t"+(String)event.magnetic.y;
  	retVal+="\tZ:\t"+(String)event.magnetic.z+"uT";
  	/*
	retVal+="\n\tminx:\t "+(String)minx+"uT";
	retVal+="\n\tminy:\t "+(String)miny+"uT";
	retVal+="\n\tminz:\t "+(String)minz+"uT";
	retVal+="\n\tmaxx:\t "+(String)maxx+"uT";
	retVal+="\n\tmaxy:\t "+(String)maxy+"uT";
	retVal+="\n\tmaxz:\t "+(String)maxz+"uT";
	retVal+="\n\tx offset:\t "+(String)((maxx+minx)/2)+"uT";
	retVal+="\n\ty offset:\t "+(String)((maxy+miny)/2)+"uT";
	retVal+="\n\tz offset:\t "+(String)((maxz+minz)/2)+"uT";

*/
                                             
  	retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\tcap (°)  :\t"+(String)headingDegrees;
	//retVal+="\n\tvitesse (N)  :\t"+(String)speed;*/
	return retVal;
}

char* SensorQMC5883::eepromParamtoString(){

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

char* SensorQMC5883::toNmea(){	
	return toNmea(getHeadingDegrees(event),getSpeed());
}

char* SensorQMC5883::toNmea(float HeadingDegrees,float Speed){
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


String SensorQMC5883::getSensorDetail(){
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

 float SensorQMC5883::getHeadingDegrees(sensors_event_t event,
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

float SensorQMC5883::getSpeed(){
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
sensors_event_t  SensorQMC5883::getEvent(){
	_LOG_PRINT(V,"SensorQMC5883 getEvent :","Start");
	mag.getEvent(&event);
	_LOG_PRINT(V,"SensorQMC5883 getEvent :","end");
	return event;
}

 void SensorQMC5883::getData(){
	//Serial.println("getData start mLastTime :"+(String)mLastTime);
	unsigned long myTime=millis();
	_LOG_PRINT(V,"getData getEvent :","Start");
	int result=mag.read(&event.magnetic.x,&event.magnetic.y,event.magnetic.z);
	swich(result){
		
//  - 0:success
	case:0
	break;
//  - 1:data too long to fit in transmit buffer
	case:1
	_LOG_PRINT(E,"getData:","data too long to fit in transmit buffer");
	break;
//  - 2:received NACK on transmit of address
	case:2
	_LOG_PRINT(V,"getData :","received NACK on transmit of address");
	break;
//  - 3:received NACK on transmit of data
	case:3
	_LOG_PRINT(V,"getData :","received NACK on transmit of data");
	break;
//  - 4:other error
	case:4
	_LOG_PRINT(V,"getData :","other error");

	break;
//  - 8:overflow (magnetic field too strong)
	case:9
	_LOG_PRINT(V,"getData :","overflow (magnetic field too strong");

	break;
	}
	_LOG_PRINT(V,"getData read :","end");
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


SensorQMC5883::~SensorQMC5883() {
	// TODO Auto-generated destructor stub
}

