#ifndef EEPROM_STREAM_H
#define EEPROM_STREAM_H

#include <EEPROM.h>
#define ENTETE_COMMANDE "$"
#define LG_ENTETE_COMMANDE 1
#define SEP ","
/*
template <typename T>
class mypair {
    T values [2];
  public:
    mypair (T first, T second)
    {
      values[0]=first; values[1]=second;
    }
};*/


template <class T>
 class EepromSrtream{
 	public: union{
 	 	 T datas;
 	 	 byte byteBuffer[sizeof(datas)];
 	 	 
 	}eepromBuff;
	public: EepromSrtream(){
		EEPROM.begin(sizeof(eepromBuff));
		Serial.print(" EEPROM stream initializer for ");
		Serial.print(sizeof(eepromBuff));
		Serial.println(" bytes");

	}
	
	/**********************************************/
public: T* readEeprom(){
	  for (int i=0; i< sizeof(eepromBuff);i++){
		eepromBuff.byteBuffer[i]=EEPROM.read(i);
	  }
	  return &eepromBuff.datas;
	}
	/*****************************************************/
public: void writeEewriprom(){
	  for (int i=0; i< sizeof(eepromBuff);i++){
		EEPROM.write(i,eepromBuff.byteBuffer[i]);
	  }
	  EEPROM.commit();
	}
	/***************************************************/
public: 

	String toString(){
	  readEeprom();
	  String rep="";
	  rep +="\n x= " +String(eepromBuff.datas.ini.x);
	  rep +="\t y= " +String(eepromBuff.datas.ini.y);
	  rep +="\t z= "+String(eepromBuff.datas.ini.z);
	  return rep;
	}
};
#endif