#include "counters_generator.h"

void counters_generator::processLine(const String& line)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("Błąd parsowania JSON w linii: ");
    Serial.println(line);
    return;
  }

  appendToVector(doc["list1"].as<JsonArray>(), list1);
  appendToVector(doc["list2"].as<JsonArray>(), list2);
  appendToVector(doc["list3"].as<JsonArray>(), list3);
}

void counters_generator::appendToVector(const JsonArray& array, std::vector<String>& vec)
{
  if (array.isNull())
    return;
  for (const char* item : array)
  {
    vec.push_back(String(item));
  }
}