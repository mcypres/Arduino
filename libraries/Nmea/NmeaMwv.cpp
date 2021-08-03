/*NmeaMwv.cpp
 *
 *  Created on: 11/12/2019
 *      Author: marc
 */

#include <NmeaMwv.h>
#include <avr/dtostrf.h>


NmeaMwv::NmeaMwv(Logger* myLogger):Nmea(myLogger){
}


char* NmeaMwv:: toNmea(int heading,float speed){
/*
MWV	Wind Speed and Angle
       1   2 3   4 
$--MWV,x.x,a,x.x,a*hh

	1 Wind Angle, 0 to 360 degrees
	2 Reference, R = Relative, T = True
	3 Wind Speed
	4 Wind Speed Units, K/M/N
	(5 Status, A = Data Valid)
	5(6) Checksum
VWR  Relative Wind Speed and Angle
       1   2 3   4 5   6 7   8 9
       |   | |   | |   | |   |
$--VWR,x.x,a,x.x,N,x.x,M,x.x,K*hh

	1 Wind direction
 	2 Wind direction
	3 Speed
	4 N = Knots
	5 Speed
	6 M = Meters Per
	7 Speed
	8 K = Kilometers
	9 Checksum
*/
	static char msg[]=NMEA_MWV_MSG;
	char tmp[10];
	//Serial.print("toNmea start");
	_LOG_PRINT(V," toNmea start",msg);
 	dtostrf((float)heading,4, 1,tmp);
	strncpy(msg+BREARING_OFFSET,tmp,4);
	_LOG_PRINT(V,"heading",heading);
	dtostrf((float) speed,5,2,tmp);
	strncpy(msg+SPEED_OFFSET,tmp,5);
	_LOG_PRINT(V,"speed",speed);
	checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
	_LOG_PRINT(V,"speed",speed);
	return msg;
}

