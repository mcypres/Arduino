/*
 * ut61e.cpp
 *
 *  Created on: 1 août 2015
 *      Author: marc
 */

#include "ut61e.h"

ut61e::ut61e(HardwareSerial *inStream) {
	mInStream=inStream;
	mInStream->begin(19200,SERIAL_7O1);

}
int ut61e::getData(){
	int rep=0;
	while((mInStream->available()>0)&&(rep==0)){
	//if(mInStream->available()>0){
		// get the new byte:
		char inChar = (char)mInStream->read();
/*		Serial.print(inChar);*/
		// add it to the msgin:
		msgIn+= inChar;
/*		Serial.print("lastMsgIn\t");
		Serial.println(lastMsgIn);*/
		if (inChar == '\n'&& msgIn.length()>0) {
			rep=1;
			lastMsgIn=msgIn;
/*			Serial.println(lastMsgIn);*/
			msgIn="";
		}
    }
    return rep;
  }
  void  ut61e::decode(){
  	  unit=lastMsgIn.charAt(UNIT_POS);
  	  echelle=lastMsgIn.charAt(ECHELLE_POS);
 /* 	  Serial.print("echelle\t");
  	  Serial.println(int(echelle),HEX);
  	  Serial.print("int(unit)-int('0')\t");
  	  Serial.println(int(unit)-int('0'));*/
  	  int mBase=base[int(unit)-int('0')];
 /*  	  Serial.print("mBase\t");
  	  Serial.println(mBase,HEX);*/
  	  int decimalSep=mBase+int(echelle)-int('0');
  	 /* Serial.print("decimalSep\t");
  	  Serial.println(decimalSep);*/
  	  int sgn=int(lastMsgIn.charAt(INFO_POS));
/*  	  Serial.print("sgn1\t");
  	  Serial.println(sgn,BIN);*/
  	  sgn=sgn & SGN_BIT;
 /*  	  Serial.print("SGN_BIT\t");
  	  Serial.println(SGN_BIT,BIN);
  	  Serial.print("sgn2\t");
  	  Serial.println(sgn,BIN);*/
 	  String strvalue=(lastMsgIn.substring(START_POS_VALUE,decimalSep+1)+DECIMAL_CHAR+lastMsgIn.substring(decimalSep+1,END_POS_VALUE));
  	  
 /* 	  Serial.print("strvalue\t");
  	  Serial.println(strvalue);*/
  	  value=strvalue.toFloat();
  	  if (sgn>0)value=-value;
  }
  String ut61e::getSelection(){                                               
  	 /* Serial.print("int(echelle)-int('0')\t");
  	  Serial.println(int(unit)-int('0'));*/
  	  return selection[int(unit)-int('0')];
  }
  
  
  
  
  
  
  
  
  
void ut61e::checksum(unsigned char* ptr,char* ptChk,int msgLength){
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
     // Serial.print("  tmp :");
      //Serial.println(tmp);
      strncpy((char*)ptChk,tmp,3);
}
/*String ut61e::getSensorDetail(){
	return sensorName;
}*/
/*ut61e::~ut61e() {
	ErrFlag=false;
	ErrorMsg="";
}*/

