/*
 * Sensor.cpp
 *
 *  Created on: 1 août 2015
 *      Author: marc
 */

#include "Sensor.h"
#include <stdio.h>
#include <stdlib.h>
//namespace sensors {

Sensor::Sensor() {
	// TODO Auto-generated constructor stub

}
void Sensor::checksum(unsigned char* ptr,char* ptChk,int msgLength){
    unsigned char checksum;

      checksum = 0;
    //  ptr      = buffer;
      if ( *ptr == '$' ) ptr++;
      //while ( *ptr /* &&  *ptr != '\r'  &&  *ptr != '\n'  &&  *ptr != '*'*/ ) {
        for (int i=0;i<msgLength;i++){
       /*   Serial.print("i =");
          Serial.print(i);
          Serial.print("  *ptr :");
          Serial.print((char)*ptr);
          Serial.print("  checksum :");*/
          checksum ^= *ptr;
         // Serial.println(checksum,HEX);
          ptr++;

      }
      char tmp[3];
      itoa((int)checksum,tmp,HEX);
     //    Serial.print("  tmp :");
      //Serial.println(tmp);
      strncpy((char*)ptChk,tmp,3);
}
Sensor::~Sensor() {
	// TODO Auto-generated destructor stub
}

//} /* namespace sensors */
