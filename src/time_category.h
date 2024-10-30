#pragma once

#undef min
#undef max
#include "DateTime.h"

#include <vector>

struct time_counter
{
  time_counter(String _name)
  : current_minutes(0)
  , sum_minutes(0)
  , start_time()
  , name(_name)
  {}
  uint16_t current_minutes;
  uint16_t sum_minutes;
  DateTime start_time;
  String name;
};

class time_category
{
  public:
  String get_name(uint8_t position);
  time_counter& get_current_counter();
  void update_current_time(DateTime& current_time);
  void reset_current_time();
  void clear_sum_times();
  uint8_t get_size();
  void add_counter(time_counter counter);
  bool check_min_position();
  bool check_max_position();
  uint8_t actual_position;

  private:
  std::vector<time_counter> counters;
};