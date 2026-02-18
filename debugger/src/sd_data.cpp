/*This file presents all the neccessary logic to create a data packet for efficient data transmission */

#include "sd_data.h"

extern const char* DIR_LOADCELL = "/LoadCell";
extern const char* DIR_SHT      = "/SHT45";
extern const char* DIR_BC = "/BOOTCOUNT";
extern const char* DIR_PARAM = "/PARAM";

bool Init_SD() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Init Failed!");
        return false;
    }
    
    if (!SD.exists(DIR_LOADCELL)) SD.mkdir(DIR_LOADCELL);
    if (!SD.exists(DIR_SHT)) SD.mkdir(DIR_SHT);
    if (!SD.exists(DIR_BC)) SD.mkdir(DIR_BC);
    if (!SD.exists(DIR_PARAM)) SD.mkdir(DIR_PARAM);

    String lcPath = String(DIR_LOADCELL) + "/data.csv";
    if (!SD.exists(lcPath)) {
        File f = SD.open(lcPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Load_Value"); 
            f.close();
            Serial.println("Created LC file");
        }
    }

    String shtPath = String(DIR_SHT) + "/data.csv";
    if (!SD.exists(shtPath)) {
        File f = SD.open(shtPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Temperature,Humidity"); 
            f.close();
            Serial.println("Created SHT file");
        }
    }

    String BCPath = String(DIR_BC) + "/count.txt";
    if (!SD.exists(BCPath)) {
        File f = SD.open(BCPath, FILE_WRITE);
        if (f) {
            f.close();
            Serial.println("Created BOOTCOUNT file");
        }
    }

    String parPath = String(DIR_PARAM) + "/data.csv";
    if (!SD.exists(parPath)) {
        File f = SD.open(parPath, FILE_WRITE);
        if (f) {
            f.println("Timestamp,Transmission_No"); 
            f.close();
            Serial.println("Created PARAMETER file");
        }
    }
    return true;
}

void LoadCell_write(char* time, float load) {
    String path = String(DIR_LOADCELL) + "/data.csv";
    File dataFile = SD.open(path.c_str(), FILE_APPEND);
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
    File dataFile = SD.open(path.c_str(), FILE_APPEND);
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

void parameter(char* time){
    String path = String(DIR_PARAM) + "/data.csv";
    File dataFile = SD.open(path.c_str(), FILE_APPEND);
    if (dataFile){
        dataFile.print(time);
        dataFile.close();
        Serial.println("Parameter File Saved");
    }else{
        Serial.println("Error writing Parameter");
    }
}

void SD_Sleep() {
    SD.end(); 
}
