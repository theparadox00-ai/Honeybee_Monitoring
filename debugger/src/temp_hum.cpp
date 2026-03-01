#include "temp_hum.h"

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

static char errorMessage[64];
static int16_t error;

void T_S_sensor::init() {
    Wire.begin();
    sht4.begin(Wire, SHT45_I2C_ADDR_44);
    sht4.softReset();
    delay(10);
    
    uint32_t serialNumber = 0;
    error = sht4.serialNumber(serialNumber);
    if (error != NO_ERROR) {
        Serial.print("SHT45 serialNumber error: ");
        errorToString(error, errorMessage, sizeof(errorMessage));
        Serial.println(errorMessage);
    }
}

void T_S_sensor::readTempHum(float &temp, float &hum) {
    delay(20);  
    error = sht4.measureHighPrecision(temp, hum);
    if (error != NO_ERROR) {
        Serial.print("SHT45 measure error: ");
        errorToString(error, errorMessage, sizeof(errorMessage));
        Serial.println(errorMessage);
        temp = 0.0f;
        hum = 0.0f;
        return;
    }
}

