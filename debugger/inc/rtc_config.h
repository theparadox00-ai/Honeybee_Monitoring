#ifndef RTC_CONFIG_H
#define RTC_CONFIG_H

#include "config.h"

bool initRTC();
void Alarm_();
char* Time_(char* buffer);
bool clock_Synchronization();
bool isNightTime(char* timeStr);
void handleNightSleep(char* timeStr);

#endif