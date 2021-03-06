#ifndef DECODRER_H
#define DECODRER_H

#include "Arduino.h"
#include"Logger.h"
#include <EEPROM.h>
#include <EepromSrtream.h>
#define ENTETE_COMMANDE "$"
#define LG_ENTETE_COMMANDE 1
#define SEP ","
#define NB_PARAM 4

struct IniField{
  float x;
  float y;
  float z;
  float speedCoef;
};
union EepromBuff{
  IniField ini;
  //byte byteBuffer[sizeof(IniField)];
  float vect[3];
};

class Decoder{
	
private:
	EepromSrtream <EepromBuff> *mptEepromSrtream;
 	EepromBuff* ptEepromBuff;
 	Logger* myLogger;
 	
public:
 	Decoder(EepromSrtream <EepromBuff> *ptEepromSrtream,Logger* myLogger);
	void decodeCommande(String msg);
	String toString( EepromBuff* in);
	String toString();
	EepromBuff* getEepromBuff(){
		//return ptEepromBuff;
		return mptEepromSrtream->readEeprom();
	}
};
#endif