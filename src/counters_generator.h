#pragma once

#include "sd_card.h"
#include "time_category.h"

#include <ArduinoJson.h>

class counters_generator
{
  public:
  counters_generator(time_category& work, time_category& meeting, time_category& chill)
  : work(work)
  , meeting(meeting)
  , chill(chill)
  {}

  void processLine(const String& line);

  private:
  void appendToVector(const JsonArray& array, time_category& vec);

  time_category& work;
  time_category& meeting;
  time_category& chill;
};