#pragma once

#include "sd_card.h"

#include <ArduinoJson.h>
#include <vector>

class counters_generator
{
  public:
  counters_generator(std::vector<String>& list1, std::vector<String>& list2, std::vector<String>& list3)
  : list1(list1)
  , list2(list2)
  , list3(list3)
  {}

  void processLine(const String& line);

  private:
  void appendToVector(const JsonArray& array, std::vector<String>& vec);

  std::vector<String>& list1;
  std::vector<String>& list2;
  std::vector<String>& list3;
};