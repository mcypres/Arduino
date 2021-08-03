
/*SensorSerialNmeaRmc.h
 * Marc Cypres
 * 10/10/2020
 * */
 
#ifndef SENSOR_SERIAL_NMEA_RMC_H
#define SENSOR_SERIAL_NMEA_RMC_H

#include "SensorSerialNmea.h"
#define NB_FIELD 11
class SensorSerialNmeaRmc: public SensorSerialNmea {
public:
	SensorSerialNmeaRmc(String sensorName,
				HardwareSerial *ptOutputSer,
				Logger *myLogger,
				MqttInterface* interface=NULL);
	boolean publish(char* msg);
protected:
      	char* getTopicName(int i);

//$GPRMC,202312.00,A,3952.16458,S,07310.73268,W,0.117,,101020,,,A*74
/*
 *    1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum
*/
	//char topicName[NB_FIELD][12]={"Time Stamp","Validity","Latitude","NS","Longitude","EW","Vitesse","Cap","Date","Dev Mag","Dev EW"};

	char topicName[NB_FIELD][12]={"","","Latitude","NS","Longitude","EW","Vitesse","Cap","","",""};
};
#endif
