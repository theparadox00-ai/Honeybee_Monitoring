#include "loadcell.h"

HX711_ADC LoadCell(HX711_DOUT_PIN, HX711_SCK_PIN);

float LoadCellReading(int bootCount, long &savedOffset) {
    LoadCell.begin();
    
    bool _tare = (bootCount == 0);
    unsigned long stabilizingTime = 3000;
    
    LoadCell.start(stabilizingTime, _tare);
    LoadCell.setCalFactor(CALIBRATION_FACTOR); 
    
    if (_tare) {
        savedOffset = LoadCell.getTareOffset();
        Serial.print("New Zero Point Saved: ");
        Serial.println(savedOffset);
    } else {
        LoadCell.setTareOffset(savedOffset);
        Serial.print("Zero Point Restored: ");
        Serial.println(savedOffset);
    }
    
    float weight = LoadCell.getData();
    
    LoadCell.powerDown();  
    return weight;
}
