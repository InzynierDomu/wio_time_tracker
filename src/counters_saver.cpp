#include "counters_saver.h"

void counters_saver::parse(String& line)
{
  auto count = counters.get_size();
  for (size_t i = 0; i < count; i++)
  {
    line += name;
    line += "_";
    line += counters.get_name(i);
    line += ": ";
    line += counters.get_sum_minutes(i);
    if (i < count - 1)
    {
      line += "\n";
    }
  }
}