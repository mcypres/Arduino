/*
 * SensorHMC5883Multi.h
 *
 *  Created on: 27 mai. 2019
 *      Author: marc
 */

#ifndef SensorHMC5883Multi_H_
#define SensorHMC5883Multi_H_

 class SensorHMC5883Multi :protected SensorHMC5883{
 private:
 	 int port;
 	 
 public:SensorHMC5883Multi(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					float declinationAngleDegres,
					float declinationAnglemin,
					float xOffset,
					float yOffset,
					float zOffset,
					float speedCoef,
					int id
					int port):SensorHMC5883(ptOutputSer,
							myLogger,
							declinationAngleDegres,
							declinationAnglemin,
							xOffset,
							yOffset,
							zOffset,
							speedCoef,
							id)
 
 	this.port=port;
 	
 	 
 }
 #endif