
#include <Wire.h>
#include <math.h>
#include "QMC5883L.h"
#include "HardwareSerial.h"

/*
 * QMC5883L
 * http://wiki.epalsite.com/images/7/72/QMC5883L-Datasheet-1.0.pdf
 */


static void write_register( int addr, int reg, int value )
{
 pinMode(PIN_SYNC,OUTPUT);
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

static int read_register( int addr, int reg, int count )
{
  //digitalWrite(PIN_SYNC,true);
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
 // digitalWrite(PIN_SYNC,false);
  Wire.requestFrom(addr,count);
  int n = Wire.available();
  if(n!=count) return 0;

  return n;
}

void QMC5883L::reconfig()
{
  write_register(addr,QMC5883L_CONFIG,oversampling|range|rate|mode);  
}

void QMC5883L::reset()
{
  digitalWrite(PIN_SYNC,true);
  write_register(addr,QMC5883L_RESET,0x01);
  digitalWrite(PIN_SYNC,false);
  reconfig();
}

void QMC5883L::setOversampling( int x )
{
  switch(x) {
    case 512:
      oversampling = QMC5883L_CONFIG_OS512;
      break;
    case 256:
      oversampling = QMC5883L_CONFIG_OS256;
      break;
    case 128:
      oversampling = QMC5883L_CONFIG_OS128;
      break;
    case 64:
      oversampling = QMC5883L_CONFIG_OS64;
      break;
  } 
  reconfig();
}

void QMC5883L::setRange( int x )
{
  switch(x) {
    case 2:
      range = QMC5883L_CONFIG_2GAUSS;
      break;
    case 8:
      range = QMC5883L_CONFIG_8GAUSS;
      break;
  }
  reconfig();
}

void QMC5883L::setSamplingRate( int x )
{
  switch(x) {
    case 10:
      rate = QMC5883L_CONFIG_10HZ;
      break;
    case 50:
      rate = QMC5883L_CONFIG_50HZ;
      break;
    case 100:
      rate = QMC5883L_CONFIG_100HZ;
      break;
    case 200:
      rate = QMC5883L_CONFIG_200HZ;
      break;
  }
  reconfig();
}

void QMC5883L::init() {
  /* This assumes the wire library has been initialized. */
  addr = QMC5883L_ADDR;
  oversampling = QMC5883L_CONFIG_OS512;
  range = QMC5883L_CONFIG_2GAUSS;
  rate = QMC5883L_CONFIG_50HZ;
  mode = QMC5883L_CONFIG_CONT;
  reset();
}

int QMC5883L::ready()
{
  if(!read_register(addr,QMC5883L_STATUS,1)) return 0;
  uint8_t status = Wire.read();
  return status & QMC5883L_STATUS_DRDY; 
}

int QMC5883L::readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t )
{
  int i=0;
  //Serial.println("*******readRaw start****");
  while(!ready()) {
  	i++;
  	if (i>10000)
  		Serial.println("*******timeOver****");
  		return -1;
  	}
//Serial.println("*******ready ok****");	
  if(!read_register(addr,QMC5883L_X_LSB,6)) return 0;
//Serial.println("*******readRegister ok****");
  *x = Wire.read() | (Wire.read()<<8);
  *y = Wire.read() | (Wire.read()<<8);
  *z = Wire.read() | (Wire.read()<<8);
 // Serial.println("*******readRaw end ****");

  return 1;
}

void QMC5883L::resetCalibration() {
  xhigh = yhigh = 0;
  xlow = ylow = 0;
}

int QMC5883L::readHeading()
{
  int16_t x, y, z, t;

  if(!readRaw(&x,&y,&z,&t)) return 0;

  /* Update the observed boundaries of the measurements */

  if(x<xlow) xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow) ylow = y;
  if(y>yhigh) yhigh = y;

  /* Bail out if not enough data is available. */
  
  if( xlow==xhigh || ylow==yhigh ) return 0;

  /* Recenter the measurement by subtracting the average */

  x -= (xhigh+xlow)/2;
  y -= (yhigh+ylow)/2;

  /* Rescale the measurement to the range observed. */
  
  float fx = (float)x/(xhigh-xlow);
  float fy = (float)y/(yhigh-ylow);

  int heading = 180.0*atan2(fy,fx)/M_PI;
  if(heading<=0) heading += 360;
  
  return heading;
}
