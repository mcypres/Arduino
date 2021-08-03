#ifndef MWC_QMC5883
#define MWC_QMC5883

#include "Arduino.h"
#include "Wire.h"

#include"Logger.h" 
#define QMC5883_ADDR 0x0D


//REG CONTROL

//0x09

#define Mode_Standby    0b00000000
#define Mode_Continuous 0b00000001

#define ODR_10Hz        0b00000000
#define ODR_50Hz        0b00000100
#define ODR_100Hz       0b00001000
#define ODR_200Hz       0b00001100

#define RNG_2G          0b00000000
#define RNG_8G          0b00010000

#define OSR_512         0b00000000
#define OSR_256         0b01000000
#define OSR_128         0b10000000
#define OSR_64          0b11000000

#define RESET_VALUE 0x01
#define SOFT_RESET_VALUE 0x80

//register address

#define SATUS_REG1		0X06	//Status Register
#define TEMPERATURE_REG_OUT1	0X07	//Temperature Sensor Output1
#define TEMPERATURE_REG_OUT2	0X08	//Temperature Sensor Output2
#define CONTROL_REG1		0X09	//Control Registe1
#define CONTROL_REG2		0X0A	//Control Register2
#define SET_RESET_PERIODE_REG	0x0B	//SET/RESET Period Register
#define CHIP_REG		0x0D	//Chip ID Register

class MechaQMC5883{
public:

MechaQMC5883(Logger* myLogger);
void setAddress(uint8_t addr);

void init(uint16_t mode=Mode_Continuous,uint16_t odr=ODR_200Hz,uint16_t rng=RNG_8G,uint16_t osr=OSR_512); //init qmc5883

void setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr); // setting

void softReset(); //soft RESET

int read(int* x,int* y,int* z); //reading
int read(int* x,int* y,int* z,int* a);
int read(int* x,int* y,int* z,float* a);

float azimuth(int* a,int* b);
protected:
Logger *myLogger;

private:

void WriteReg(uint8_t Reg,uint8_t val);
uint8_t address = QMC5883_ADDR;

};



#endif
