
#include <TestNmea.h>
//#include <alloca.h>
	TestNmea::TestNmea(HardwareSerial *ptOutputSer,
					Logger *myLogger,
					 char* msgModel,
					 const int portList[],
					 const int posList[],
					 const float coefList[],
					 int listSyze):Sensor(ptOutputSer,myLogger){
	
		this->msgModel= msgModel;
		this->portList=portList;
		this->posList=posList;
		this->listSyze=listSyze;
		this->coefList=coefList;
		msgCksLenght=strlen(msgModel)-6;
		CksPos=msgModel+(strlen(msgModel)-4);
		analogReference(DEFAULT);
	}
	

	String TestNmea::toString(){
		
		int lenght=7;
		int precis=2;
		//char *msg=alloca(strlen(msgModel));
		//strncpy(msg,msgModel,strlen(msgModel));
		
		for(int i=0;i<listSyze;i++){
			char tmp[10];
			int mes=analogRead(portList[i]);
			float val=coefList[i]*(float)mes;
			 dtostrf(val,lenght,precis,tmp);
			 strncpy(msgModel+posList[i],tmp,lenght);
			 _LOG_PRINT(V,"msgModel",(char)* msgModel);
		}
//		checksum((unsigned char *) msg,msg+strlen(msg),msg+strlen(msg));
			
		return msgModel;
	}
	char* TestNmea::toNmea(){
		int lenght=7;
		int precis=2;
		_LOG_PRINT(V,"msgModel",(char *)msgModel);
		//Serial.println((char*)msgModel);
		//char *msg=alloca(strlen(msgModel));
		//strncpy(msg,msgModel,strlen(msgModel));
	//	_LOG_PRINT(M,"msg initial",(char)*msgModel);
		//Serial.print("msgModel initial");
		//printstring(msgModel);
		for(int i=0;i<listSyze;i++){
			char tmp[10];
			int mes=analogRead(portList[i]);
			float val=coefList[i]*(float)mes;
			_LOG_PRINT(D,"coefList",coefList[i]);
			_LOG_PRINT(D,"port",portList[i]);
			_LOG_PRINT(D,"mes",mes);
			_LOG_PRINT(D,"val",val);
			 dtostrf(val,lenght,precis,tmp);
			 strncpy(msgModel+posList[i],tmp,lenght);
			  printstring(msgModel);;
		}
		int len=strlen(msgModel);
		checksum((unsigned char *) msgModel,CksPos,msgCksLenght);
			
		return (char *)msgModel;
	}
	void TestNmea::getData(){
	}
	/*static */void TestNmea::printstring(char * str){
		int len=strlen(str);
		Serial.print("len : ");
		Serial.println(len);
		for (int i=0;i<len;i++){
			Serial.print(str[i]);
		}
		Serial.println("");
	}