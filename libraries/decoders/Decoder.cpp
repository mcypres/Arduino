
#include<Decoder.h>

Decoder::Decoder(EepromSrtream <EepromBuff> *ptEepromSrtream,Logger* myLogger){
 	 mptEepromSrtream=ptEepromSrtream;
 	 this->myLogger=myLogger;
 	 
 	 
}
void Decoder::decodeCommande(String msg){
	_LOG_PRINT(D,"decodeCommande  msg= ",msg);
	int startPos=0;
	int endPos=0;
	for(int i=0; i<NB_PARAM;i++){
		endPos=msg.indexOf(SEP,startPos);
		if(endPos<0){
			endPos=msg.length();
		} 

		_LOG_PRINT(D,"startPos",startPos);
		_LOG_PRINT(D,"endPos",endPos);
		_LOG_PRINT(D,"substring(startPos,endPos)",msg.substring(startPos,endPos));
		
		mptEepromSrtream->eepromBuff.datas.vect[i]=msg.substring(startPos,endPos).toFloat();
		startPos=endPos+1;
	}
	//mptEepromSrtream->writeEewriprom( &eepromBuff);
	mptEepromSrtream->writeEewriprom();
	_LOG_PRINT(D,"toString()",toString());

}
	/**************************************************/
String Decoder::toString( EepromBuff* in){
	String rep="";
	rep +="\n x= " +String(in->ini.x);
	rep +="\t y= " +String(in->ini.y);
	rep +="\t z= "+String(in->ini.z);
	rep +="\t speedCoef= "+String(in->ini.speedCoef);
	
	return rep;
}
	/**************************************************/
String Decoder::toString(){
	EepromBuff* in= mptEepromSrtream->readEeprom();
	//Serial.println(toString(in));
	return toString(in);
}
