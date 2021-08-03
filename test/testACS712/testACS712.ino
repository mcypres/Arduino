#define NBENTRY 4 
#define NMEA_INT_MSG "$PAINT, ,      ,A, ,      ,A, ,      ,A, ,      ,A*     "
//********************012345678901234567890123456789012345678901234567890123456789
#define FIRST_BLOCK_OFFSET 7 
#define SIGNE_OFFSET 0
#define VALUE_OFFSET 2
#define VALUE_SAVE_CHAR 8
#define BLOCK_LENGTH 11
#define MSG_LENGTH_T_CHECKSUM 49
#define CHECKSUM_OFFSET 51
#define ZERO_OFFSET 512
#define FULL_SCALE 1024

 int analogPin[] = {8,9,10,11};
 float analogSens[]={66,66,66,66};
 int val = 0;
 float refVoltage=4.985; 
 boolean first=true;
void setup() {
 
  // put your setup code here, to run once:
  analogReference(DEFAULT);
   val = analogRead(analogPin[0]);    // read the input pin
   
  Serial.begin(115200);
}

void loop() {
  if (first==true){
  int blockPos=FIRST_BLOCK_OFFSET;
  
  char msg[]=NMEA_INT_MSG;
  Serial.println( msg);
  for(int i=0;i<NBENTRY;i++){
    float current;
    char saveChar;
    val = analogRead(analogPin[i])-ZERO_OFFSET;    // read the input pin
    if(val<0){
      msg[blockPos+SIGNE_OFFSET]='-';
    }
    current=(((val+0.5)*refVoltage)/FULL_SCALE);
    char tmp[10];
    dtostrf(abs(current),5, 3,tmp);
    int len=strlen(tmp);
    if (len>5) len=5;
    strncpy(msg+blockPos+VALUE_OFFSET,tmp,len);
    
    blockPos+=BLOCK_LENGTH;
  }
  checksum((unsigned char *) msg,msg+CHECKSUM_OFFSET,MSG_LENGTH_T_CHECKSUM);
   Serial.println(msg);
   first=false;
  }
 // delay(1000);


}
