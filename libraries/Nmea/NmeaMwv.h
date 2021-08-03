/*NmeaMwv.h
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */

#ifndef NMEA_MWV
#define NMEA_MWV

#include <Nmea.h>
//#include"Logger.h"

#define NMEA_MWV_MSG "$PAMWV,000.0,R,000.00,N*     "
//********************0123456789012345678901234567890
#define BREARING_OFFSET 7
#define SPEED_OFFSET 15
#define MSG_LENGTH_T_CHECKSUM 22
#define CHECKSUM_OFFSET 24

class NmeaMwv :public Nmea {
public:
	NmeaMwv(Logger* myLogger);
	char* toNmea(int heading,float speed);
};

#endif
