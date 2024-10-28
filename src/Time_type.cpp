#include "time_type.h"

String time_type::get_name(uint8_t position)
{
  return counters[position].name;
}

spend_time& time_type::get_current_counter()
{
  return counters[actual_position];
}

void time_type::update_current_time(DateTime& current_time)
{
  spend_time& current = get_current_counter();

  TimeSpan time_diff = current_time - current.start_time;
  uint16_t minutes = static_cast<uint16_t>(time_diff.totalseconds() / 60);

  if (minutes != current.current_minutes)
  {
    current.current_minutes = minutes;
    current.sum_minutes++;
  }
}

void time_type::reset_current_time()
{
  spend_time& current = get_current_counter();
  current.current_minutes = 0;
}

void time_type::clear_sum_times()
{
  for (auto& counter : counters)
  {
    counter.sum_minutes = 0;
  }
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