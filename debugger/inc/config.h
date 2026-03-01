#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "FS.h"
#include "SD.h"

#include <HX711_ADC.h>    // Weight sensing for bee hive 
#include <RTClib.h>       // DS3231 Timekeeping 
#include <Wifi.h>
#include <SensirionI2cSht4x.h> // Precision Temp/Humidity 
#include <ESP_Mail_Client.h> // Email alerts for swarming 
#include <LiFuelGauge.h> 

// ESP32 Pins Configuration
#define SD_CS_PIN 13    // Standard CS IO D11
#define HX711_DOUT_PIN 3    // Example GPIO for HX711 D2
#define HX711_SCK_PIN  38    // Example GPIO for HX711 D3
#define CLOCK_INTERRUPT_PIN 14 // Clock Sleep Interrupt D10
#define MOSFET_PIN 21 // Mosfet Key

// System Constants
#define CALIBRATION_FACTOR 404.70 // Replace with your field-tested value [
#define SLEEP_DURATION 30     // Logging interval for bee behavior
#define LONG_DURATION 28800ULL * 1000000ULL // Sleep at the Night Time


#endif





