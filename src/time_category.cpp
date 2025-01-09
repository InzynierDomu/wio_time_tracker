#include "time_category.h"

String time_category::get_name(uint8_t position)
{
  return counters[position].name;
}

uint16_t time_category::get_sum_minutes(uint8_t position)
{
  return counters[position].sum_minutes;
}

time_counter& time_category::get_current_counter()
{
  return counters[actual_position];
}

time_counter& time_category::get_counter(uint8_t position)
{
  return counters[position];
}

void time_category::update_current_time(DateTime& current_time)
{
  time_counter& current = get_current_counter();

  TimeSpan time_diff = current_time - current.start_time;
  uint16_t minutes = static_cast<uint16_t>(time_diff.totalseconds() / 60);

  if (minutes != current.current_minutes)
  {
    current.current_minutes = minutes;
    current.sum_minutes++;
  }
}

void time_category::reset_current_time()
{
  time_counter& current = get_current_counter();
  current.current_minutes = 0;
}

void time_category::clear_sum_times()
{
  for (auto& counter : counters)
  {
    counter.sum_minutes = 0;
  }
}

uint8_t time_category::get_size()
{
  return counters.size();
}

void time_category::add_counter(time_counter counter)
{
  counters.push_back(counter);
}

bool time_category::check_min_position()
{
  if (actual_position > 0)
  {
    return true;
  }
  return false;
}

bool time_category::check_max_position()
{
  if (actual_position < counters.size())
  {
    return true;
  }
  return false;
}