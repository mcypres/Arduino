        
#include "Wire.h"
#define TCAADDR         0x70
//#define  QMC5883_ADRS 0x1E // 0x0D ?????
#define  QMC5883_ADRS 0xD 
#define WIRE_ADDRESS_ERROR   2
#define WIRE_NACKORERROR_ERROR   3

void setup() {
    Serial.begin(115200);
    Serial.println();    
    Serial.println();
    Wire.begin();
  // tcaSelect(0); 
}

void loop() {
        Wire.beginTransmission(QMC5883_ADRS);
        Wire.write(0xAA);          
        Wire.endTransmission();
}

  void    tcaSelect(uint8_t i)
    {
        byte error;

        if ((i < 0) || (i > 7)) {
            return;
        }

        Wire.beginTransmission(TCAADDR);
        Wire.write(1 << i);
        error = Wire.endTransmission();
        switch (error) {
        case 0:
            break;
        case WIRE_ADDRESS_ERROR:
            Serial.println("    ****Wire address error");
            break;
        case WIRE_NACKORERROR_ERROR:
            Serial.println("    ****Wire NAK or ERROR");
            break;
        default:
            Serial.print("    ****Wire unknown error:");
            Serial.println(error);
            break;
        }
    }
