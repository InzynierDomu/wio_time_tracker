#include "counters_saver.h"

counters_saver::counters_saver(String& name, time_category counters, const std::function<void(const String&)>& saver)
{
  StaticJsonDocument<256> doc;
  doc["mode"] = name;
  doc["name"] = counters.get_name(0);
  doc["sum_minutes"] = counters.get_sum_minutes(0);

  String jsonString;
  serializeJson(doc, jsonString); // Serializuj do stringa
  saver(jsonString);
}