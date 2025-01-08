#pragma once

#include "sd_card.h"
#include "time_category.h"

#include <ArduinoJson.h>

class counters_generator
{
  public:
  counters_generator(time_category& list1, time_category& list2, time_category& list3)
  : list1(list1)
  , list2(list2)
  , list3(list3)
  {}

  void processLine(const String& line);

  private:
  void appendToVector(const JsonArray& array, time_category& vec);

  time_category& list1;
  time_category& list2;
  time_category& list3;
};