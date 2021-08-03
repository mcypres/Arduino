#include "Nmea.h"

Nmea::Nmea(Logger *myLogger,char* msgFrame,int nbParam,float* paramList,signed char* paramSize,unsigned char* paramPrec,int paramPos[]){
	msg=msgFrame;
	this->myLogger=myLogger;
	this->paramList=paramList;
	this->nbParam=nbParam;
 	this->paramSize=paramSize;
 	this->paramPrec=paramPrec;	
 	this->paramPos=paramPos;
 	//char *strchr(const char *str, int c) 
 	//firstParamPos=strchr(msg, ',')+1 ;
 	//paramPos++;
	checksumPos=strchr(msg, '*')+1 ;
	//_LOG_PRINT(M," *checksumPos",*(checksumPos)-1);
	//_LOG_PRINT(M," msg constructor",msg);
}

char* Nmea::ToNmea(){
	_LOG_PRINT(D," msg","msg");
	//char* paramPos=firstParamPos; 
	for(int i=0;i<nbParam;i++){
	  char tmp[10];
	 // _LOG_PRINT(M," msg",msg);
	  dtostrf(paramList[i],paramSize[i], paramPrec[i],tmp);
	 /* _LOG_PRINT(M,"  tmp :",tmp);
	  _LOG_PRINT(M,"  paramSize[i] :",paramSize[i]);
	  _LOG_PRINT(M,"  paramPos[i] :",paramPos[i]);*/
	  strncpy(msg+paramPos[i],tmp,paramSize[i]);
	 // paramPos=strchr(paramPos, ',')+1 ;
	 //_LOG_PRINT(M," msg fin for ",msg);
	 
	}
	 // _LOG_PRINT(M," msg avant checksum",msg);	  
	 // _LOG_PRINT(M,"  checksumPos-msg :",checksumPos-msg);
	  checksum((unsigned char *)msg, checksumPos,(checksumPos-msg)-2);
	  //_LOG_PRINT(M," msg apres checksum",msg);
	  return msg;

}
void Nmea::checksum(unsigned char* ptr,char* ptChk,int msgLength){
     unsigned char checksum;
     checksum = 0;
     if ( *ptr == '$' ) ptr++;
     for (int i=0;i<msgLength;i++){
 
		_LOG_PRINT(V,"i =",i);
		_LOG_PRINT(V,"  *ptr :",(char)*ptr);
		_LOG_PRINT(V,"  checksum :",checksum);
		
       checksum ^= *ptr;
       //Serial.println(checksum,HEX);
       
       _LOG_PRINT_HEX(V,"  checksum :",checksum);
       ptr++;
     }
     char tmp[3];
     itoa((int)checksum,tmp,HEX);
#ifdef DEBUG     
     Serial.print("  tmp :");
     Serial.println(tmp);
#endif
     strncpy((char*)ptChk,tmp,3);

}