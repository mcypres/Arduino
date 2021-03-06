/************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here





#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <stdio.h>
#define MSG_Max_LENGHTS      85
#define NMEA_SEPARATOR ","
#define NMEA_HEADER "$"
#define AIS_HEADER "!"
#define MSG_TAIL "*"
#define SEATALK_HAEDER "#"
#define NMEA_MPT_HEADER "PAMPR,"
#define PRESSURE_UNIT "hPa,"
#define TEMP_UNIT ",°C"
#define NMEA_MPT_MSG "$PAMPR,00000.00,hPa,000.00,°C*     "
//********************0123456789012345678901234567890
#define PRESSION_OFFSET 8
#define TEMP_OFFSET 20
#define MSG_LENGTH_T_CHECKSUM 28
#define CHECKSUM_OFFSET 31
#define BARO_TIME_INTERVAL 1000


Adafruit_BMP085 bmp;

boolean state=false;  
unsigned long lastTime=0;

void checksum(unsigned char* ptr,char* ptChk){  
       unsigned char checksum;

        checksum = 0;
      //  ptr      = buffer;
        if ( *ptr == '$' ) ptr++;
        //while ( *ptr /* &&  *ptr != '\r'  &&  *ptr != '\n'  &&  *ptr != '*'*/ ) {
          for (int i=0;i<MSG_LENGTH_T_CHECKSUM;i++){
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
        Serial.print("  tmp :");
        Serial.println(tmp);
        strncpy((char*)ptChk,tmp,3);
        
} 

String getNmeaMPTmsg(){
  float pression,temperature;
  char msg[]=NMEA_MPT_MSG;
  char tmp[10];
  pression=((float)bmp.readPressure())/100;
  dtostrf(pression,7, 2,tmp);
  strncpy(msg+PRESSION_OFFSET,tmp,7);
  dtostrf((float) bmp.readTemperature(),5,2,tmp);
  strncpy(msg+TEMP_OFFSET,tmp,5);
  checksum((unsigned char *) msg,(char*)msg+CHECKSUM_OFFSET);
  return msg;

}

/****************************************************/
/****     Initialisation                          ***/ 
/****************************************************/ 

void setup() {
  Serial3.begin(9600);    // Bluetooth
  Serial2.begin(4800);    // NMEA
  //Serial1.begin(4800);    // Seatalk
  Serial.begin(9600);     // USB
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

  
}

/****************************************************/
/****     Main loop                               ***/ 
/****************************************************/ 


void loop() {
  float pression,temperature;
  String msg;
  char tmp[10];
  String msgTemp="";
  // copy from BT to USB
    if (Serial3.available()>0) {
     // digitalWrite(13, state);
     // state=!state;
      int inByte = Serial3.read();
      Serial.write(inByte);
    }
    // copy fron USB to BT
    if (Serial.available()>0) {
     // digitalWrite(13, state);
     // state=!state;
      int inByte = Serial.read();
      Serial3.write(inByte);
    }
    // copy from NMEA to USB and BT
      
    if (Serial2.available()>0) {
    //  digitalWrite(13, state);
   //   state=!state;
      int inByte = Serial2.read();
      Serial3.write(inByte);
      Serial.write(inByte);
    }
 
    //send MPT message 
    if (lastTime +BARO_TIME_INTERVAL < millis()){
        digitalWrite(13, state);
        state=!state;
        lastTime =millis();
        msg=getNmeaMPTmsg();
        Serial3.println(msg);
        Serial.println(msg);
        Serial.println();
    }
    //delay(5);
}

/****************************************************/
/*********BMP085 Barometric Pressure & Temp Sensor***/ 
/****************************************************/


/*
String getNmeaMPTmsg(){
  float pression,temperature;
  char msg[]=NMEA_MPT_MSG;
  char tmp[10];
  pression=((float)bmp.readPressure())/100;
  dtostrf(pression,7, 2,tmp);
  strncpy(msg+PRESSION_OFFSET,tmp,7);
  dtostrf((float) bmp.readTemperature(),5,2,tmp);
  strncpy(msg+TEMP_OFFSET,tmp,5);
  checksum((unsigned char *) msg,(char*)msg+CHECKSUM_OFFSET);
  return msg;

}

*/


  
  
  
  
  
  
