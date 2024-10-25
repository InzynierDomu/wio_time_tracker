#pragma once

#undef min
#undef max
#include "DateTime.h"

#include <vector>

struct spend_time
{
  spend_time(String _name)
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

class time_type
{
  public:
  String get_name(uint8_t position);
  spend_time& get_current_counter();
  uint8_t get_size();
  void add_counter(spend_time counter);
  bool check_min_position();
  bool check_max_position();
  uint8_t actual_position;

  private:
  std::vector<spend_time> counters;
};