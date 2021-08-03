/*SensorGirouette.h
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */

#ifndef SENSOR_GIROUETTE_H
#define SENSOR_GIROUETTE_H

#include <Sensor.h>
#include <SensorMagneto.h>
#include "tca.h"

class SensorGirouette :public Sensor {
public:
	SensorGirouette(String sensorName,Logger* myLogger,SensorMagneto* sensor1,SensorMagneto* sensor2);
	float getHeading();
	String toString();
	void getData();
	void init(uint8_t range);
protected:
	SensorMagneto* sensor1;
	SensorMagneto* sensor2;
	boolean dual;
	float Heading;
};
#endif


