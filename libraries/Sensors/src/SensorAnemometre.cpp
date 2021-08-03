/*SensorAnemometre.cpp
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */


#include <SensorAnemometre.h>
volatile unsigned long GanemoCount;
boolean state = true;
void anemoInterupt() {
  GanemoCount++;
  if (state) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  state = !state;
}
SensorAnemometre::SensorAnemometre(String sensorName,Logger* myLogger) : Sensor( sensorName,myLogger){
  pinMode(ANEMO_PIN, INPUT);
  attachInterrupt(ANEMO_PIN, anemoInterupt, RISING);
 
} 

float SensorAnemometre::getSpeed(){
	// get anemo data
	getData();
	return speed;
}

String SensorAnemometre::toString(){
	return "";
}

void SensorAnemometre::getData(){
	unsigned long myTime=millis();
	if (lastTime +TIME_INTERVAL_ANEMO  <myTime){		
		if ((myTime-lastTime)>0){
			speed=(GanemoCount*1000)/(myTime-lastTime);
		}
		else{
			speed=-1;
		}
		GanemoCount=0;
		// update time	
		lastTime =myTime;
	}
}





