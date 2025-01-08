#pragma once

#include "time_category.h"

#include <ArduinoJson.h>
#include <functional>

class counters_saver
{
  public:
  counters_saver(String& name, time_category counters, const std::function<void(const String&)>& saver);

  private:
};
