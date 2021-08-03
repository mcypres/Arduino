    #define NUM_TCA         1
#include "Wire.h"
    #define WIRE_ADDRESS_ERROR   2
    #define WIRE_NACKORERROR_ERROR   3
    #define TCAADDR         0x70
    #define  HMC5883_ADRS 0x1E
    void   tcaSelect(uint8_t i);
    void   tcaDisable(uint8_t i);
    uint8_t   tcaScan(void);
 
 void setup() {
    Serial.begin(115200);
    Serial.print("tcaScan strart");
    Serial.print(tcaScan());
    }
    
void loop() {
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
            Serial.println("Wire address error");
            break;
        case WIRE_NACKORERROR_ERROR:
            Serial.println("Wire NAK or ERROR");
            break;
        default:
            Serial.print("Wire unknown error:");
            Serial.println(error);
            break;
        }
    }

    void
    tcaDisable(uint8_t i)
    {
        Wire.beginTransmission(TCAADDR + i);
        Wire.write(0);            // no channel selected
        Wire.endTransmission();
    }

    uint8_t
    tcaScan(void)
    {
      unsigned int  mask = 0x00;

        Wire.begin();

        for (auto t = 0; t < 8 /*NUM_TCA*/; t++) {
            byte error, address;
            int nDevices;

            Serial.print("TCA Port ");
            Serial.print(t); 
            Serial.print(" = ");           
            Serial.println( (TCAADDR + t),HEX);
            tcaSelect(t);

            nDevices = 0;
            for (address = 1; address < 127; address++ ) {
                Wire.beginTransmission(address);
                error = Wire.endTransmission();

                if (!error) {
                    if ((address >= TCAADDR) && (address <= (TCAADDR + t))) {
                        continue;
                    }
                    if (address == HMC5883_ADRS) {
                        Serial.printf("  HMC5883 found\n");
                        mask |= (1 << t);
                    }
                    else {
                        Serial.print("  Unknown I2C device found at address ");
                        Serial.println(address,HEX);
                    }
                    nDevices++;
                }
                else if (error != WIRE_ADDRESS_ERROR) {
                    Serial.print("  Unexpected I2C device error: ");
                    Serial.print( error);
                    Serial.print("  at address ");
                    Serial.println( address ,HEX);
 
                }
            }
            if (nDevices == 0) {
                Serial.print("No I2C devices found\n");
            }
        }

        return mask;
    }
