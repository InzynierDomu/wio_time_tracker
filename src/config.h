namespace config
{
String counter_list_path = "/config.json";
String wifi_config_path = "/wifi.json";
unsigned int localPort = 2390; // local port to listen for UDP packets
char timeServer[] = "tempus1.gum.gov.pl"; // extenral NTP server e.g. time.nist.gov
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
} // namespace config