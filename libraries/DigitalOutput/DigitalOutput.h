#include "Arduino.h"
#include"Logger.h"
#define TIME_OUT 1500

class DigitalOutput {
private:
	Logger *myLogger;
	int * mPortList;
	long * mTimeout;
	int mNbPort; 
public:
	DigitalOutput(String sensorName,Logger *myLogger,int nbPort,int * portList);
	void refresh();
	void write(String msg);
	~DigitalOutput();
};
