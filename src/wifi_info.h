#pragma once

#include <ArduinoJson.h>
#include <Seeed_FS.h>

struct wifi_info
{
  String ssid;
  String pass;
  // adjust time for timezone offset in secs +/- from UTC
  // WA time offset from UTC is +8 hours (28,800 secs)
  // + East of GMT
  // - West of GMT
  long timezone;


  void deserializeWiFiConfig(File& file)
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, file);

    ssid = doc["wifi"]["ssid"].as<String>();
    pass = doc["wifi"]["password"].as<String>();
    timezone = doc["timezone"].as<long>();
  }
};
