#pragma once

#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "config.h"
#include "wifi_info.h"

#include <rpcWiFi.h>

class time_clock
{
  public:
  void init(wifi_info& info);
  DateTime get_now();

  private:
  unsigned long getNTPtime();
  unsigned long sendNTPpacket(const char* address);

  RTC_SAMD51 rtc;
  WiFiClient client;
  WiFiUDP udp;
  byte packetBuffer[config::NTP_PACKET_SIZE];
};
