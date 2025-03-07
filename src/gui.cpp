#include "gui.h"

#include "graph/logo.h"

void gui::init()
{
  m_screen.begin();
  m_screen.setRotation(3);
  m_screen.setTextColor(TFT_BLACK);
  m_screen.fillScreen(TFT_WHITE);
}

void gui::refresh_left_side(bool is_running, time_counter& counter, DateTime date_time)
{
  clear_part_screen(is_running, 0, 0, 180, 240);
  print_date_time(is_running, date_time);
  print_time(is_running, counter);
}

void gui::print_side_menu(time_category& counters)
{
  m_screen.fillRect(180, 0, 160, 240, TFT_LIGHTGREY);

  int posY = 8 + (counters.actual_position * 26);
  m_screen.fillRect(180, posY, 160, 26, TFT_DARKGREY);

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  print_list_counters(counters);
}

void gui::print_time(bool is_running, time_counter& counter)
{
  m_screen.setFreeFont(&FreeSansBold24pt7b);

  const int screen_center_x = 90;

  int current_minutes_width = m_screen.textWidth(String(counter.current_minutes));
  int sum_minutes_width = m_screen.textWidth(String(counter.sum_minutes));

  int current_minutes_x = screen_center_x - (current_minutes_width / 2);
  int sum_minutes_x = screen_center_x - (sum_minutes_width / 2);

  clear_part_screen(is_running, screen_center_x - 50, 50, 100, 60);
  clear_part_screen(is_running, screen_center_x - 50, 110, 100, 60);

  m_screen.drawString(String(counter.current_minutes), current_minutes_x, 50);
  m_screen.drawString(String(counter.sum_minutes), sum_minutes_x, 110);
}

void gui::print_date_time(bool is_running, DateTime date_time)
{
  uint8_t hour = date_time.hour();
  uint8_t minute = date_time.minute();
  uint8_t day = date_time.day();
  uint8_t month = date_time.month();

  String formattedHour = format_two_digits(hour);
  String formattedMinute = format_two_digits(minute);
  String formattedDay = format_two_digits(day);
  String formattedMonth = format_two_digits(month);

  String text = formattedHour + ":" + formattedMinute;

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  clear_part_screen(is_running, 60, 185, 110, 110);

  m_screen.drawString(text, 60, 185);

  text = formattedDay + "." + formattedMonth;
  m_screen.drawString(text, 60, 210);
}

void gui::print_welcome()
{
  print_logo(20, 100);
  m_screen.setFreeFont(&FreeSansBold18pt7b);
  m_screen.drawString("Loading...", 80, 120);
}

void gui::print_list_counters(time_category& counters)
{
  uint8_t posY = 10;
  for (size_t i = 0; i < counters.get_size(); i++)
  {
    m_screen.drawString(counters.get_name(i), 185, posY);
    posY += 26;
  }
}

void gui::clear_part_screen(bool is_running, const uint16_t position_x, const uint16_t position_y, const uint16_t width,
                            const uint16_t height)
{
  if (is_running)
  {
    m_screen.fillRect(position_x, position_y, width, height, TFT_DARKGREEN);
  }
  else
  {
    m_screen.fillRect(position_x, position_y, width, height, TFT_WHITE);
  }
}

String gui::format_two_digits(uint8_t number)
{
  return (number < 10) ? "0" + String(number) : String(number);
}

void gui::print_logo(const uint16_t position_x, const uint16_t position_y)
{
  m_screen.startWrite();
  for (int j = 0; j < logo_id::height; j++)
  {
    for (int i = 0; i < logo_id::width; i++)
    {
      uint16_t color = pgm_read_word(&logo_id::bitmap[j * logo_id::width + i]);
      m_screen.drawPixel(position_x + i, position_y + j, color);
    }
  }
  m_screen.endWrite();
}