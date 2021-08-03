/*
 * SensorAnalog.h
 *
 *  Created on: 5 oct. 2015
 *      Author: marc
 */

#ifndef SRC_SENSORANALOG_H_
#define SRC_SENSORANALOG_H_
#include "Arduino.h"
#include "Sensor.h"

class SensorAnalog : public Sensor {
protected:
	int mPinNum;
	float mCoefConv;
	float Value;
public:
	SensorAnalog(int pinNum,float refVoltage);
	String toString();
	String toNmea();
	void getData();
	float getData();
	virtual ~SensorAnalog();
	int getPinNum() const {
		return mPinNum;
	}
	float getValue() const {
		return Value;
	}
};

#endif /* SRC_SENSORANALOG_H_ */
