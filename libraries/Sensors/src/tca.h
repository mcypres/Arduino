/*
 * tca.h
 *
 *  Created on: 31 juil. 2019
 *      Author: marc
 */

#ifndef TCA_H_
#define TCA_H_

#include <Wire.h>

#define TCAADDR 0x70
#define NB_SENSOR 2

/*public*/ class Tca{
public:
	static void tcaselect(uint8_t i) {
	  if (i > NB_SENSOR) return;
	  //Wire.begin();
	  Wire.beginTransmission(TCAADDR);
	  Wire.write(1 << i);
	  Wire.endTransmission();  
	};
};
#endif
