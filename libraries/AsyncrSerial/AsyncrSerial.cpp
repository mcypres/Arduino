#include <AsyncrSerial.h>

AsyncrSerial::AsyncrSerial(HardwareSerial* serPort,Logger* myLogger){
	ptSerPort=serPort;
	isValidData=false;
	this->myLogger=myLogger;
	inputBuff.reserve(INPUT_BUFF_SIZE);
}
char * AsyncrSerial::getMessage(){
	if(isValidData){
		isValidData=false;
		inputBuff = "";
	}
	
	//****************!!!!!!!!!!!!!no _LOG_PRINT  in the while loop exept error case !!!!!!!!!!!!****************
	
	while (ptSerPort->available() > 0) {
		// get the new byte:
		char inChar = (char)ptSerPort->read();
		// add it to the msgin:
		inputBuff += inChar;
		//if (inChar == '\n' && inputBuff.length() > 0) {
		 
		if (inputBuff.length() > 66||inChar == '\r') {
			isValidData=true;
      		return inputBuff.c_str();
		}
		if(inputBuff.length() > INPUT_BUFF_SIZE-3){
			_LOG_PRINT(M,F("inputBuff  overflow "),inputBuff);
			inputBuff="";
			
		}
    }
    return NULL;
}


bool AsyncrSerial::GetIsValidData(){

	return isValidData;
}
