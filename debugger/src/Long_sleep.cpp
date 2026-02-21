#include "Long_sleep.h"

struct SunTimes {
  bool   flag;
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
  String url = "https://api.sunrisesunset.io/json?lat=" + String(lat, 6) + "&lng=" + String(lon, 6) +"&timezone=auto";

  http.begin(url);
  int httpCode = http.GET();
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

  if (result.sunrise.length() < 5 || result.sunset.length() < 5) {
    Serial.println("Invalid sunrise/sunset strings");
    result.ok = false;
  } else {
    result.ok = true;
  }

  return result;
}
