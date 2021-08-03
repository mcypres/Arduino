/*
 * SensorMagneto.cpp
 *
 *  Created on: 10/12/2019
 *      Author: marc
 */

#include <SensorMagneto.h>

SensorMagneto::SensorMagneto(String sensorName,Logger* myLogger):Sensor( sensorName,myLogger){

	//SensorMagneto(0,0,0,0,0,0);
	xOffset=0;
	yOffset=0;
	zOffset=0;
	minx=MAX_VALUE;
	miny=MAX_VALUE;
	minz=MAX_VALUE;
	maxx=MIN_VALUE;
	maxy=MIN_VALUE;
	maxz=MIN_VALUE;
}

SensorMagneto::SensorMagneto(String sensorName,
				Logger* myLogger,
				int declinationAngleDegres=0,
				float declinationAnglemin=0,
				float xOffset=0,
				float yOffset=0,
				float zOffset=0,int id=0)
				 :Sensor( sensorName,myLogger){

	this->xOffset=xOffset;
	this->yOffset=yOffset;
	this->zOffset=zOffset;
	//mag.setMagGain(HMC5883_MAGGAIN_8_1);
	minx=MAX_VALUE;
	miny=MAX_VALUE;
	minz=MAX_VALUE;
	maxx=MIN_VALUE;
	maxy=MIN_VALUE;
	maxz=MIN_VALUE;
 	//GanemoCount=0;
}
String SensorMagneto::toString(sensors_event_t event){
	
	String retVal;
	//mag.getEvent(&event);

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



  	/*retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\n\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\n\tcap (°)  :\t"+(String)headingDegrees;
	retVal+="\n\tvitesse (N)  :\t"+(String)speed;*/
	return retVal;
}


String SensorMagneto::getSensorDetail(sensors_event_t event){
 	sensor_t sensor;
	String retVal;
  	retVal= "\n\tSensor:\t"+(String)sensor.name;
  	retVal+="\n\tDriver Ver:\t"+(String)sensor.version;
  	retVal+="\n\tUnique ID:\t"+(String)sensor.sensor_id;
  	retVal+="\n\tMax Value:\t"+(String)sensor.max_value+" uT";
 	retVal+="\n\tMin Value:\t"+(String)sensor.min_value+" uT";
  	retVal+="\n\tResolution:\t"+(String)sensor.resolution+" uT";
 	return retVal;
}
 float SensorMagneto::getHeadingDegrees(sensors_event_t event,float declinationAngle=0,float xOffset=0, float yOffset=0){
	 float mHeading=atan2(event.magnetic.y-yOffset, event.magnetic.x-xOffset) + declinationAngle;
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

 void SensorMagneto::ComputeData(sensors_event_t event,float declinationAngle=0){
	//Serial.println("getData start lastTime :"+(String)lastTime);
		magnetudexy = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x));
  		magnetude = sqrt(sq(event.magnetic.y) + sq(event.magnetic.x) + sq(event.magnetic.z));
		heading = atan2(event.magnetic.y, event.magnetic.x) + declinationAngle;
		//Serial.println("getData magnetude :"+(String)magnetude);
	// Correct for when signs are reversed.
		if (heading < 0)
			heading += 2 * PI;
	// Check for wrap due to addition of declination.
		if (heading > 2 * PI)
    			heading -= 2 * PI;
	// Convert radians to degrees for readability.
  		headingDegrees = heading * 180 / M_PI;	
}

SensorMagneto::~SensorMagneto() {
	// TODO Auto-generated destructor stub
}

/*
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
} */
/*
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
 	return "no data";
}*/
