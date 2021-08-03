/*
 * SensorHMC5883.cpp
 *
 *  Created on: 31 juil. 2015
 *      Author: marc
 */


// Nmea message definition
//$--MWV,x.x,a,x.x,a*hh

#define NMEA_MWV_MSG "$PAMWV,000.0,R,000.00,N*     "
//********************0123456789012345678901234567890
#define BREARING_OFFSET 7
#define SPEED_OFFSET 15
#define MSG_LENGTH_T_CHECKSUM 22
#define CHECKSUM_OFFSET 24
#define GIRO_TIME_INTERVAL 1000
// anemotetre param definition
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 3
#define MAX_VALUE 10000
#define MIN_VALUE -10000

SensorHMC5883::SensorHMC5883(float declinationAngleDegres,float declinationAnglemin,float xOffset,float yOffset,float zOffset,int id) {

#ifdef ANALOG_INPUT
	Serial.println(" ANALOG_INPUT");
	 cap=new SensorAnalog(1,(float)360);
	 vitesse=new SensorAnalog(2,(float)360);
#endif
	Serial.println("HMC5883 start");
	Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(id);
	Serial.println("HMC5883 created");
	declinationAngle = (declinationAngleDegres + (declinationAnglemin / 60)) * M_PI / 180;
	lastTime=0;
	sensorName="HMC5883";
	ErrFlag=false;
	if (!mag.begin()) {
		ErrorMsg="Could not find a valid HMC5883 sensor, check wiring!";
		Serial.println(ErrorMsg);
		ErrFlag=true;
		return;
	  }
	 Serial.println("SensorHMC5883 ok"); 
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
 	GanemoCount=0;
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



  	/*retVal+="\n\tmagxy(uT):\t"+(String)magnetudexy;
  	retVal+="\n\tmag(uT)  :\t"+(String)magnetude;
	retVal+="\n\tcap (°)  :\t"+(String)headingDegrees;
	retVal+="\n\tvitesse (N)  :\t"+(String)speed;*/
	return retVal;
}

String SensorHMC5883::toNmea(){
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
	char msg[]=NMEA_MWV_MSG;
	char tmp[10];
	//Serial.print("toNmea start");
#ifdef	  ANALOG_INPUT

	//Serial.print("cap getValue():");
	//Serial.println (cap->getValue());
	dtostrf(cap->getValue(),4, 1,tmp);
	//Serial.print("tmp: ");
	//Serial.println(tmp);
#else
	Serial.print("toNmea  elseif");
 	dtostrf(getHeadingDegrees(),4, 1,tmp);
#endif

	strncpy(msg+BREARING_OFFSET,tmp,4);
	Serial.println(msg);
	
#ifdef	  ANALOG_INPUT
	//Serial.print("vitesse getValue():");
	//Serial.println (vitesse->getValue());

	dtostrf(vitesse->getValue(),5, 2,tmp);
	//Serial.print("tmp: ");
	//Serial.println(tmp);
#else	
	  dtostrf((float) getSpeed(),5,2,tmp);
#endif
	  strncpy(msg+SPEED_OFFSET,tmp,5);
	Serial.print("speed:");
	Serial.println(speed);
	  checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
	  return msg;
}
String SensorHMC5883::getSensorDetail(){
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
float SensorHMC5883::getHeadingDegrees(){
	mag.getEvent(&event);
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
float SensorHMC5883::getSpeed(){
	unsigned long myTime=millis();
	float mSpeed=-1;
	if ((myTime-lastTime)>0){
		mSpeed=(GanemoCount*1000)/(myTime-lastTime);
	}
	GanemoCount=0;
	// update time	
	lastTime =myTime;
	return mSpeed;
}
 void SensorHMC5883::getData(){
	//Serial.println("getData start lastTime :"+(String)lastTime);
	unsigned long myTime=millis();
	//if (lastTime +GIRO_TIME_INTERVAL <myTime){
		mag.getEvent(&event);
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
	// get anemo data	
		if ((myTime-lastTime)>0){
			speed=(GanemoCount*1000)/(myTime-lastTime);
		}
		else{
			speed=-1;
		}
		GanemoCount=0;
		// update time	
	        lastTime =myTime;
	//}
	
}


SensorHMC5883::~SensorHMC5883() {
	// TODO Auto-generated destructor stub
}

