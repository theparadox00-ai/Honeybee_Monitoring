/* This file presents all the necessary logic to create SD directories/files and log data for efficient transmission. */

#include "sd_data.h"

const char* DIR_LOADCELL = "/LoadCell";
const char* DIR_SHT      = "/SHT45";
const char* DIR_BC       = "/BOOTCOUNT";
const char* DIR_PARAM    = "/PARAM";
const char* DIR_TEMP     = "/Temp";

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

    String lcPath = String(DIR_LOADCELL) + "/data.csv";
    if (!SD.exists(lcPath)) {
        File f = SD.open(lcPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Load_Value");
            f.close();
            Serial.println("Created LC file");
        } else {
            Serial.println("Failed to create LC file");
        }
    }

    String shtPath = String(DIR_SHT) + "/data.csv";
    if (!SD.exists(shtPath)) {
        File f = SD.open(shtPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Temperature,Humidity");
            f.close();
            Serial.println("Created SHT file");
        } else {
            Serial.println("Failed to create SHT file");
        }
    }

    String BCPath = String(DIR_BC) + "/count.txt";
    if (!SD.exists(BCPath)) {
        File f = SD.open(BCPath, FILE_WRITE);
        if (f) {
            f.close();
            Serial.println("Created BOOTCOUNT file");
        } else {
            Serial.println("Failed to create BOOTCOUNT file");
        }
    }

    String parPath = String(DIR_PARAM) + "/data.csv";
    if (!SD.exists(parPath)) {
        File f = SD.open(parPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Transmission_No");
            f.close();
            Serial.println("Created PARAMETER file");
        } else {
            Serial.println("Failed to create PARAMETER file");
        }
    }

    String lcPath_temp = String(DIR_TEMP) + "/lc_temp.csv";
    if (!SD.exists(lcPath_temp)) {
        File f = SD.open(lcPath_temp, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Load_Value");
            f.close();
            Serial.println("Created LC temp file");
        } else {
            Serial.println("Failed to create LC temp file");
        }
    }

    String shtPath_temp = String(DIR_TEMP) + "/sht_temp.csv";
    if (!SD.exists(shtPath_temp)) {
        File f = SD.open(shtPath_temp, FILE_WRITE);
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
    String path = String(DIR_LOADCELL) + "/data.csv";
    File dataFile = SD.open(path.c_str(), FILE_WRITE); 
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
    String path = String(DIR_SHT) + "/data.csv";
    File dataFile = SD.open(path.c_str(), FILE_WRITE);
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
    String lcPath_temp = String(DIR_TEMP) + "/lc_temp.csv";
    File dataFile = SD.open(lcPath_temp.c_str(), FILE_WRITE);
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
    String shtPath_temp = String(DIR_TEMP) + "/sht_temp.csv";
    File dataFile = SD.open(shtPath_temp.c_str(), FILE_WRITE);
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
    String path = String(DIR_PARAM) + "/data.csv";
    File dataFile = SD.open(path.c_str(), FILE_WRITE);
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

