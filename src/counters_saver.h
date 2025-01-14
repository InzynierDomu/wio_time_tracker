#pragma once

#include "time_category.h"

#include <functional>

class counters_saver
{
  public:
  counters_saver(time_category& counters, String name)
  : counters(counters)
  , name(name)
  {}
  void parse(String& json);

  private:
  time_category& counters;
  String name;
};
