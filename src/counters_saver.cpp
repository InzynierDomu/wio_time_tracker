#include "counters_saver.h"

void counters_saver::parse(String& json)
{
  auto count = counters.get_size();
  for (size_t i = 0; i < count; i++)
  {
    json += counters.get_name(i);
    json += counters.get_sum_minutes(i);
    if (i < count - 1)
    {
      json += "\n";
    }
  }
}