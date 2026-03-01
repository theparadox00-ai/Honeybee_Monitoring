/* This is the main program which represents the workflow of the whole program for data logging 
   While deploying the system wakes up every 30s for getting the load cell sensor reading (Storing it in SD card)
   every 15 mins for getting the SHT45 sensor reading 
   The data is then sent to SMTP server to the Gmail every 3 hours
*/

#include "config.h"
#include "loadcell.h"
#include "temp_hum.h"
#include "rtc_config.h" 
#include "sd_data.h"
#include "file.h"
#include "transmission.h" 
#include <vector>

size_t BootCount = BootCount(DIR_BC, "/count.txt");
RTC_DATA_ATTR long savedOffset = 0;

enum B_CountNo : char {
    LoadCell = 1, 
    SHT45 = 30, 
    WI_FI = 360
};

const char* WIFI_SSID = "********";
const char* WIFI_PASS = "********";

T_S_sensor SHT45;
LiFuelGauge gauge(MAX17043, 0);

class ScheduledTask {
public:
    virtual bool shouldExecute(size_t BootCount) = 0;
    virtual void execute(char* timeStr, long savedOffset) = 0;
    virtual ~ScheduledTask() {}
};

class LoadCellTask : public ScheduledTask {
public:
    bool shouldExecute(size_t BootCount) override {
        return BootCount % LoadCell == 0;
    }
    void execute(char* timeStr, long savedOffset) override {
        float loadValue = LoadCellReading(BootCount, savedOffset);
        Serial.print("Weight: "); Serial.println(loadValue);
        LoadCell_write(timeStr, loadValue);  
    }
};

class SHT45Task : public ScheduledTask {
public:
    bool shouldExecute(size_t BootCount) override {
        return BootCount % SHT45 == 0 && BootCount != 0;  
    }
    void execute(char* timeStr, long savedOffset) override {
        SHT45.init();
        float tempValue = -999.0, humiValue = -999.0;
        SHT45.readTempHum(tempValue, humiValue);
        Serial.print(tempValue); Serial.print(","); Serial.println(humiValue);
        if (tempValue != -999.0 && humiValue != -999.0) {
            SHT_write(timeStr, tempValue, humiValue);
            Serial.println("SHT Data Logged");
        }
    }
};

class WiFiEmailTask : public ScheduledTask {
public:
    bool shouldExecute(size_t BootCount) override {
        return BootCount % WI_FI == 0 && BootCount != 0;  
    }
    void execute(char* timeStr, long savedOffset) override {
        double SoC = gauge.getSOC();
        double Voltage = gauge.getVoltage();
        
        WiFi.persistent(false);
        WiFi.mode(WIFI_STA);
        delay(100);  
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        
        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 25000) {
            Serial.print(".");
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            clock_Synchronization();
            Serial.print("SoC: "); Serial.print(SoC); Serial.print("%, Voltage: "); Serial.println(Voltage);
            if (Send_All_Data_Email((int)SoC, (int)Voltage)) {
                Serial.println("Email Sent Successfully.");
                parameter(timeStr);
            } else {
                Serial.println("Email Failed.");
            }
        } else {
            Serial.printf("WiFi Failed. Status: %d\n", WiFi.status());
        }
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }
};

void setup() {
    Serial.begin(115200);
    delay(1000); 

    if (!initRTC()) Serial.println("RTC Init Failed!");

    char timeStr[25];
    Time_(timeStr);  

    if (isNightTime(timeStr)) {
        handleNightSleep(timeStr);
        return;
    }

    if (!Init_SD()) Serial.println("SD Init Failed!");

    if (BootCount == (size_t)-1) {
        Serial.print("Invalid Count Loop Repeats again");
        esp_deep_sleep_start();
        return;
    }

    if (BootCount == 0) {
        Serial.println("Reset Tare !!!! ");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        clock_Synchronization();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        Serial.println(timeStr);
        gauge.reset();
        delay(200);
        gauge.setAlertThreshold(10);
    }

    double SoC = gauge.getSOC();
    double Voltage = gauge.getVoltage();

    Serial.print("Boot Count: "); 
    Serial.println(BootCount);
    Serial.print("SoC: "); 
    Serial.print(SoC);
    Serial.print("%, Voltage: ");
    Serial.println(Voltage);

    LoadCellTask loadCellTask;
    SHT45Task sht45Task;
    WiFiEmailTask wifiEmailTask;
    
    std::vector<ScheduledTask*> tasks = {
        &loadCellTask,
        &sht45Task,
        &wifiEmailTask
    };

    for (auto task : tasks) {
        if (task->shouldExecute(BootCount)) {
            task->execute(timeStr, savedOffset);
        }
    }

    SD_Sleep(); 
    BootCount++; 
    writeLastSent(DIR_BC, "/count.txt", BootCount);

    Serial.println("Entering deep sleep...");
    Serial.flush();
    Alarm_();
    esp_deep_sleep_start();
}

void loop() {}
