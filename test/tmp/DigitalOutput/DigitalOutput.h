/*
 * DigitalOutput.h
 *
 *  Created on: 14 juin 2016
 *      Author: marc
 */

#ifndef DIGITALOUTPUT_H_
#define DIGITALOUTPUT_H_
# include "string.h"
#include "Arduino.h"


class DigitalOutput {
private:
	int mSize;
	int *mPorts;
	int mScanMsgPos;
	long *mTimeout;
	long *mDelay;
public:
	DigitalOutput(int size,int *ports,long * delay);
	void loop();
	void decodeMsg(String * msg);
	virtual ~DigitalOutput();
private:
	int getCommand(String * msg);
};



#endif /* DIGITALOUTPUT_H_ */
