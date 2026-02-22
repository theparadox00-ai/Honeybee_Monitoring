#include "Long_sleep.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct SunTimes {
  bool   valid;      // Renamed for clarity
  String sunrise;  
  String sunset;
};

SunTimes getSunTimes(double lat, double lon) {
  SunTimes result = { false, "", "" };

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return result;
  }

  HTTPClient http;
  String url = "https://api.sunrisesunset.io/json?lat=" + String(lat, 6) + 
               "&lng=" + String(lon, 6) + "&timezone=auto";

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode <= 0) {
    Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    http.end();
    return result;
  }

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP error: %d\n", httpCode);
    http.end();
    return result;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<2048> doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return result;
  }

  result.sunrise = doc["results"]["sunrise"].as<String>();
  result.sunset  = doc["results"]["sunset"].as<String>();

  // Validate we actually got strings
  if (result.sunrise.length() == 0 || result.sunset.length() == 0) {
    Serial.println("Invalid sunrise/sunset strings");
    result.valid = false;
  } else {
    result.valid = true;
  }

  return result;
}
