#pragma once

#include "DateTime.h"
#include "TFT_eSPI.h"
#include "time_category.h"

class gui
{
  public:
  gui() {}
  void init();
  void refresh_left_side(bool is_running, time_counter& counter, DateTime date_time);
  void print_side_menu(time_category& counters);
  void print_time(bool is_running, time_counter& counter);
  void print_date_time(bool is_running, DateTime date_time);
  void print_welcome();

  private:
  void print_list_counters(time_category& counters);
  void clear_part_screen(bool is_running, const uint16_t position_x, const uint16_t position_y, const uint16_t width, const uint16_t height);
  void print_logo(const uint16_t position_x, const uint16_t position_y);
  String format_two_digits(uint8_t number);
  TFT_eSPI m_screen;
};