#include "time_type.h"

String time_type::get_name(uint8_t position)
{
  return counters[position].name;
}

spend_time& time_type::get_current_counter()
{
  return counters[actual_position];
}

uint8_t time_type::get_size()
{
  return counters.size();
}

void time_type::add_counter(spend_time counter)
{
  counters.push_back(counter);
}

bool time_type::check_min_position()
{
  if (actual_position > 0)
  {
    return true;
  }
  return false;
}

bool time_type::check_max_position()
{
  if (actual_position < counters.size())
  {
    return true;
  }
  return false;
}