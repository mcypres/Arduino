#ifndef LOGGER_H
#define LOGGER_H

#include "HardwareSerial.h"
#include "Print.h"


//#define WITH_PATH
#define SEP1 "-->\t"
#define SEP2 "-->\t***"


#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

enum LevelValue{N=0,M=1,E=2,W=3,D=4,V=5};

 class Logger{
private:
	HardwareSerial* ptLogSer;
	char* mMachineName;
	LevelValue mRequieredLevel;
	
public:
	Logger(HardwareSerial* ptLogSer,char* machinName,LevelValue level);

	
//	void printLog(int level,char* sender,char* title,const __FlashStringHelper *value);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],const String &value);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],const char* value);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],char value);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned char value, int type);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],int value, int type= DEC);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned int value, int type= DEC);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],long value, int type= DEC);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],unsigned long value, int type= DEC);

	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],float value, int type=  2);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[],double value, int type=  2);
	void printLog(LevelValue level,const char* sender,const int lineNb ,const char* title,const char tail[], Printable& value);

private: 
	void headerLog(const char* sender,const int lineNb ,const char* title);
	
};

 //Logger *myLogger;
//#define _LOG_  printLog(2,(char*)  __FILE__ ,__LINE__,
#ifndef NO_LOG
	#undef F
	#define F(x) x
	#define _LOG_PRINT(LEVEL,TITRE,VALUE) myLogger->printLog(LEVEL,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),"***\n",(VALUE))
	#define _LOG_PRINT_TAIL(LEVEL,TITRE,VALUE,TAIL) myLogger->printLog(LEVEL,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),(TAIL),(VALUE))
	#define _LOG_PRINT_HEX(LEVEL,TITRE,VALUE) myLogger->printLog(LEVEL,(char*)  __FILE__ ,__LINE__,(char*)(TITRE),"***\n",(VALUE))
#endif
#ifdef NO_LOG
	#define _LOG_PRINT(LEVEL,TITRE,VALUE) ((void)0)
	#define _LOG_PRINT_TAIL(LEVEL,TITRE,VALUE,TAIL) ((void)0)
	#define _LOG_PRINT_HEX(LEVEL,TITRE,VALUE) ((void)0)
#endif
#endif
