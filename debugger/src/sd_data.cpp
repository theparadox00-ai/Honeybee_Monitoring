#include "sd_data.h"

const char* DIR_LOADCELL = "/LoadCell";
const char* DIR_SHT      = "/SHT45";
const char* DIR_BC       = "/BOOTCOUNT";
const char* DIR_PARAM    = "/PARAM";
const char* DIR_TEMP     = "/Temp";

const char* LC_PATH      = "/LoadCell/data.csv";
const char* SHT_PATH     = "/SHT45/data.csv";
const char* BC_PATH      = "/BOOTCOUNT/count.txt";
const char* PARAM_PATH   = "/PARAM/data.csv";
const char* LC_TEMP_PATH = "/Temp/lc_temp.csv";
const char* SHT_TEMP_PATH= "/Temp/sht_temp.csv";

bool Init_SD() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Init Failed!");
        return false;
    }

    if (!SD.exists(DIR_LOADCELL)) SD.mkdir(DIR_LOADCELL);
    if (!SD.exists(DIR_SHT))      SD.mkdir(DIR_SHT);
    if (!SD.exists(DIR_BC))       SD.mkdir(DIR_BC);
    if (!SD.exists(DIR_PARAM))    SD.mkdir(DIR_PARAM);
    if (!SD.exists(DIR_TEMP))     SD.mkdir(DIR_TEMP);

    if (!SD.exists(LC_PATH)) {
        File f = SD.open(LC_PATH, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Load_Value");
            f.close();
            Serial.println("Created LC file");
        } else {
            Serial.println("Failed to create LC file");
        }
    }

    if (!SD.exists(SHT_PATH)) {
        File f = SD.open(SHT_PATH, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Temperature,Humidity");
            f.close();
            Serial.println("Created SHT file");
        } else {
            Serial.println("Failed to create SHT file");
        }
    }

    if (!SD.exists(BC_PATH)) {
        File f = SD.open(BC_PATH, FILE_WRITE);
        if (f) {
            f.close();
            Serial.println("Created BOOTCOUNT file");
        } else {
            Serial.println("Failed to create BOOTCOUNT file");
        }
    }
    
    if (!SD.exists(PARAM_PATH)) {
        File f = SD.open(PARAM_PATH, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Transmission_No");
            f.close();
            Serial.println("Created PARAMETER file");
        } else {
            Serial.println("Failed to create PARAMETER file");
        }
    }

    if (!SD.exists(LC_TEMP_PATH)) {
        File f = SD.open(LC_TEMP_PATH, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Load_Value");
            f.close();
            Serial.println("Created LC temp file");
        } else {
            Serial.println("Failed to create LC temp file");
        }
    }

    if (!SD.exists(SHT_TEMP_PATH)) {
        File f = SD.open(SHT_TEMP_PATH, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Temperature,Humidity");
            f.close();
            Serial.println("Created SHT temp file");
        } else {
            Serial.println("Failed to create SHT temp file");
        }
    }

    return true;
}

void LoadCell_write(char* time, float load) {
    File dataFile = SD.open(LC_PATH, FILE_WRITE); 
    if (dataFile) {
        dataFile.print(time);
        dataFile.print(",");
        dataFile.println(load);
        dataFile.close();
        Serial.println("LC Saved.");
    } else {
        Serial.println("Error writing LC");
    }
}

void SHT_write(char* time, float temp, float humi) {
    File dataFile = SD.open(SHT_PATH, FILE_WRITE);
    if (dataFile) {
        dataFile.print(time);
        dataFile.print(",");
        dataFile.print(temp);
        dataFile.print(",");
        dataFile.println(humi);
        dataFile.close();
        Serial.println("SHT Saved.");
    } else {
        Serial.println("Error writing SHT");
    }
}

void LoadCell_write_Temp(char* time, float load) {
    File dataFile = SD.open(LC_TEMP_PATH, FILE_WRITE);
    if (dataFile) {
        dataFile.print(time);
        dataFile.print(",");
        dataFile.println(load);
        dataFile.close();
        Serial.println("LC Temp Saved.");
    } else {
        Serial.println("Error writing LC Temp");
    }
}

void SHT_write_Temp(char* time, float temp, float humi) {
    File dataFile = SD.open(SHT_TEMP_PATH, FILE_WRITE);
    if (dataFile) {
        dataFile.print(time);
        dataFile.print(",");
        dataFile.print(temp);
        dataFile.print(",");
        dataFile.println(humi);
        dataFile.close();
        Serial.println("SHT Temp Saved.");
    } else {
        Serial.println("Error writing SHT Temp");
    }
}

void parameter(char* time, int transmissionNo) {
    File dataFile = SD.open(PARAM_PATH, FILE_WRITE);
    if (dataFile) {
        dataFile.print(time);
        dataFile.print(",");
        dataFile.println(transmissionNo);
        dataFile.close();
        Serial.println("Parameter File Saved");
    } else {
        Serial.println("Error writing Parameter");
    }
}

void SD_Sleep() {
    SD.end();
}
