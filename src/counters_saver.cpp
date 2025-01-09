#include "counters_saver.h"

void counters_saver::parse(String& json)
{
  StaticJsonDocument<256> doc;
  doc["mode"] = name;
  doc["name"] = counters.get_name(0);
  doc["sum_minutes"] = counters.get_sum_minutes(0);

  serializeJson(doc, json);
}