#include "time_clock.h"

void time_clock::init(wifi_info& info)
{
  tzOffset = info.timezone;

  char* ssid = strdup(info.ssid.c_str());
  char* pass = strdup(info.pass.c_str());
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    WiFi.begin(ssid, pass);
  }

  unsigned long ntp_time;

  ntp_time = getNTPtime();
  while (ntp_time == 0)
  {
    ntp_time = getNTPtime();
  }

  rtc.begin();
  rtc.adjust(ntp_time);
}

DateTime time_clock::get_now()
{
  return rtc.now();
}
unsigned long time_clock::sendNTPpacket(const char* address)
{
  for (int i = 0; i < config::NTP_PACKET_SIZE; ++i)
  {
    packetBuffer[i] = 0;
  }
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0; // Stratum, or type of clock
  packetBuffer[2] = 6; // Polling Interval
  packetBuffer[3] = 0xEC; // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); // NTP requests are to port 123
  udp.write(packetBuffer, config::NTP_PACKET_SIZE);
  udp.endPacket();
}

unsigned long time_clock::getNTPtime()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    udp.begin(WiFi.localIP(), config::localPort);

    sendNTPpacket(config::timeServer);
    delay(1000);

    if (udp.parsePacket())
    {
      udp.read(packetBuffer, config::NTP_PACKET_SIZE);

      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

      unsigned long secsSince1900 = highWord << 16 | lowWord;

      const unsigned long seventyYears = 2208988800UL;

      unsigned long epoch = secsSince1900 - seventyYears;

      unsigned long adjustedTime = epoch + tzOffset;
      return adjustedTime;
    }
    else
    {
      udp.stop();
      return 0;
    }
    udp.stop();
  }
  else
  {
    return 0;
  }
}
