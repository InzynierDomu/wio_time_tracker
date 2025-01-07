#include "counters_generator.h"

void counters_generator::processLine(const String& line)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    //error
    return;
  }

  appendToVector(doc["list1"].as<JsonArray>(), list1);
  appendToVector(doc["list2"].as<JsonArray>(), list2);
  appendToVector(doc["list3"].as<JsonArray>(), list3);
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