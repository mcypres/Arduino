#ifndef QMC5883L_H
#define QMC5883L_H


#define PIN_SYNC 14
#import <Arduino.h>
/* The default I2C address of this chip */
#define QMC5883L_ADDR 0x0D

/* Register numbers */
#define QMC5883L_X_LSB 0
#define QMC5883L_X_MSB 1
#define QMC5883L_Y_LSB 2
#define QMC5883L_Y_MSB 3
#define QMC5883L_Z_LSB 4
#define QMC5883L_Z_MSB 5
#define QMC5883L_STATUS 6
#define QMC5883L_TEMP_LSB 7
#define QMC5883L_TEMP_MSB 8
#define QMC5883L_CONFIG 9
#define QMC5883L_CONFIG2 10
#define QMC5883L_RESET 11
#define QMC5883L_RESERVED 12
#define QMC5883L_CHIP_ID 13

/* Bit values for the STATUS register */
#define QMC5883L_STATUS_DRDY 1
#define QMC5883L_STATUS_OVL 2
#define QMC5883L_STATUS_DOR 4

/* Oversampling values for the CONFIG register */
#define QMC5883L_CONFIG_OS512 0b00000000
#define QMC5883L_CONFIG_OS256 0b01000000
#define QMC5883L_CONFIG_OS128 0b10000000
#define QMC5883L_CONFIG_OS64  0b11000000

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000

/* Rate values for the CONFIG register */
#define QMC5883L_CONFIG_10HZ   0b00000000
#define QMC5883L_CONFIG_50HZ   0b00000100
#define QMC5883L_CONFIG_100HZ  0b00001000
#define QMC5883L_CONFIG_200HZ  0b00001100

/* Mode values for the CONFIG register */
#define QMC5883L_CONFIG_STANDBY 0b00000000
#define QMC5883L_CONFIG_CONT    0b00000001

/* Apparently M_PI isn't available in all environments. */
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif
/*
enum QMC5883Mode{QMC5883L_CONFIG_STANDBY=0b00000000,QMC5883L_CONFIG_CONT=0b00000001);

enum QMC5883Oversampling{
		QMC5883L_CONFIG_OS512=0b00000000,
		QMC5883L_CONFIG_OS256=0b01000000,
		QMC5883L_CONFIG_OS128=0b10000000,
		QMC5883L_CONFIG_OS64=0b11000000};

enum QMC5883Range {
		QMC5883L_CONFIG_2GAUSS=0b00000000,
		QMC5883L_CONFIG_8GAUSS=0b00010000};
enum QMC5883Rate {
		QMC5883L_CONFIG_10HZ=0b00000000,
		QMC5883L_CONFIG_50HZ=0b00000100,
		QMC5883L_CONFIG_100HZ=0b00001000,
		QMC5883L_CONFIG_200HZ=0b00001100};

*/
class QMC5883L {
public:
  void init();
  void reset();
  int  ready();
  void reconfig();
  
  int readHeading();
  int readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t );

  void resetCalibration();

  void setSamplingRate( int rate );
  void setRange( int range );
  void setOversampling( int ovl );
  
protected:
  int16_t xhigh, xlow;
  int16_t yhigh, ylow;
  uint8_t addr;
  uint8_t mode;
  uint8_t rate;
  uint8_t range;
  uint8_t oversampling;
};

#endif
