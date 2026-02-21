#ifndef SD_DATA_H
#define SD_DATA_H

#include "config.h"

extern const char* DIR_LOADCELL;
extern const char* DIR_SHT;
extern const char* DIR_BC;
extern const char* DIR_PARAM;
extern const char* DIR_TEMP;

bool Init_SD();
void LoadCell_write(char* time, float load);
void SHT_write(char* time, float temp, float humi);
void LoadCell_Temp_write(char* time, float load);
void SHT_Temp_write(char* time, float temp, float humi);
void parameter(char* time);
void SD_Sleep();

#endif
