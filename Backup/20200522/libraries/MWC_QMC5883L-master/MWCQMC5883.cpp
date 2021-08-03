#include "MWC_QMC5883.h"

#include <Wire.h>



 MWC_QMC5883::MWC_QMC5883(Logger* myLogger){
 	this->myLogger=myLogger;
 }
 
void MWC_QMC5883::setAddress(uint8_t addr){
  address = addr;
}

void MWC_QMC5883::WriteReg(byte Reg,byte val){
  Wire.beginTransmission(address); //start talking
  Wire.write(Reg); // Tell the HMC5883 to Continuously Measure
  Wire.write(val); // Set the Register
  int err = Wire.endTransmission();
  if (err) {
  	_LOG_PRINT(M,"Wire.endTransmission err :",err);
  }
}

void MWC_QMC5883::init(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr){
  WriteReg(SET_RESET_PERIODE_REG,RESET_VALUE);
  //Define Set/Reset period
  /* 	Serial.print("\n mode :");
	Serial.println(mode);
	Serial.print("odr : ");
	Serial.println(odr);
	Serial.print("rng :");
	Serial.println(osr);
	Serial.print("osr :");
	Serial.println(osr);*/
  setMode( mode, odr, rng, osr);
  /*
  Define
  OSR = 512
  Full Scale Range = 8G(Gauss)
  ODR = 200HZ
  set continuous measurement mode
  */
}

void MWC_QMC5883::setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr){
  WriteReg(CONTROL_REG1,mode|odr|rng|osr);
}


void MWC_QMC5883::softReset(){
  WriteReg(CONTROL_REG2,SOFT_RESET_VALUE);
}

/**
 * read values from device
 * @return status value:
 *  - 0:success
 *  - 1:data too long to fit in transmit buffer
 *  - 2:received N.ACK on transmit of address
 *  - 3:received NACK on transmit of data
 *  - 4:other error
 *  - 8:overflow (magnetic field too strong)
 */
 

int MWC_QMC5883::read(int* x,int* y,int* z){
	Serial.print("I2C address :");
	Serial.println(address,HEX);
  Wire.beginTransmission(address);
  Wire.write(0x00);
  int err = Wire.endTransmission();
  if (err) {
  	_LOG_PRINT(M,"Wire.endTransmission err :",err);
  	//return err;
  }
  Wire.requestFrom(address,(uint8_t) 7);
  *x = (int)(int16_t)Wire.read() ;
  Serial.print("*x lsb:");
	Serial.println(*x);
   *x |= (int)(int16_t)(Wire.read() << 8);
	Serial.print("*x :");
	Serial.println(*x);
  *y = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  *z = (int)(int16_t)(Wire.read() | Wire.read() << 8);
  /*	Serial.print("*x :");
	Serial.println(*x);
	Serial.print("*y :");
	Serial.println(*y);
	Serial.print("*z :");
	Serial.println(*z);*/
  byte overflow = Wire.read() ;
  	Serial.print("overflo0w :");
	Serial.println(overflow,HEX);
  return (overflow & 0x02) << 2;
}

int MWC_QMC5883::read(int* x,int* y,int* z,int* a){
  int err = read(x,y,z);
  *a = azimuth(y,x);
  return err;
}

int MWC_QMC5883::read(int* x,int* y,int* z,float* a){
  int err = read(x,y,z);
  *a = azimuth(y,x);
  return err;
}


float MWC_QMC5883::azimuth(int *a, int *b){
  float azimuth = atan2((int)*a,(int)*b) * 180.0/PI;
  return azimuth < 0?360 + azimuth:azimuth;
}
