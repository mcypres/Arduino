
#ifndef MLX90393
#define MLX90393

#include "Adafruit_MLX90393.h"
#include "Logger.h"

class  SensorMLX90393: public  Adafruit_MLX90393 {
public:
	SensorMLX90393();
	float getHeadingRad();
	int getHeadingDeg();
	int getHeadingPm();
	int getMagnitude();	
	int getMagnitudeXY();
	int getBx();
	int getBy();
	int getBz();
	bool getStatus(); 
protected:
	Logger* myLogger;
	bool status;
};
#endif
