#pragma once

namespace config
{
const String counter_list_path = "/config.json";
const String wifi_config_path = "/wifi.json";
const unsigned int localPort = 2390; // local port to listen for UDP packets
const char timeServer[] = "tempus1.gum.gov.pl"; // extenral NTP server
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
} // namespace config