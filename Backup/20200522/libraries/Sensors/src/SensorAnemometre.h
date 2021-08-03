/*SensorAnemometre.h
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */

#ifndef SENSOR_ANEMOMETRE_H_
	#define SENSOR_ANEMOMETRE_H_

#include <Sensor.h>

#define TIME_INTERVAL_ANEMO 1000
#define ANEMO_INTERUPT 1
#define ANEMO_PIN 14
#define LED_PIN 2


/*
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
}*/
class SensorAnemometre :public Sensor {
public:
	SensorAnemometre(Logger* myLogger);
	float getSpeed();
	String toString();
	void getData();
protected:
	long myTime;
	long lastTime;
	float speed;
};




#endif
