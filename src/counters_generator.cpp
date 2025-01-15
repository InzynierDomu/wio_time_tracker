#include "counters_generator.h"

void counters_generator::processLine(const String& line)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    // error
    return;
  }

  appendToVector(doc["work"].as<JsonArray>(), work);
  appendToVector(doc["meeting"].as<JsonArray>(), meeting);
  appendToVector(doc["chill"].as<JsonArray>(), chill);
}

void counters_generator::appendToVector(const JsonArray& array, time_category& vec)
{
  if (array.isNull())
    return;
  for (const char* item : array)
  {
    vec.add_counter(time_counter(String(item)));
  }
}