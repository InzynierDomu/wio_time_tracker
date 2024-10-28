#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "TFT_eSPI.h"
#include "time_type.h"

#include <Arduino.h>
#undef min
#undef max
#include <map>
#include <vector>

///< possible joystick cursor moves
enum class Cursor_action
{
  pause,
  down,
  left,
  right,
  center,
  nothing
};

///< modes which device possible running
enum class Mode
{
  work,
  meeting,
  chill
};

RTC_SAMD51 rtc;
TFT_eSPI m_screen; ///< TFT screen 320x240

bool running = false;
uint16_t checkt_time = 30000;
DateTime m_date;
unsigned long last_loop_time = millis();

const unsigned long debounce_delay = 500;

Mode m_mode = Mode::chill;

time_type counters_work;
time_type counters_meetings;
time_type counters_chill;

std::map<Mode, time_type> times;
void print_list_counters(time_type& counters)
{
  uint8_t posY = 10;
  for (size_t i = 0; i < counters.get_size(); i++)
  {
    m_screen.drawString(counters.get_name(i), 185, posY);
    posY += 26;
  }
}
void print_side_menu(time_type& counters)
{
  m_screen.fillRect(180, 0, 160, 240, TFT_LIGHTGREY);

  int posY = 8 + (counters.actual_position * 26);
  m_screen.fillRect(180, posY, 160, 26, TFT_DARKGREY);

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  print_list_counters(counters);
}

void clear_part_screen(const uint16_t position_x, const uint16_t position_y, const uint16_t width, const uint16_t height)
{
  if (running)
  {
    m_screen.fillRect(position_x, position_y, width, height, TFT_DARKGREEN);
  }
  else
  {
    m_screen.fillRect(position_x, position_y, width, height, TFT_WHITE);
  }
}

void print_time(spend_time& counter)
{
  m_screen.setFreeFont(&FreeSansBold24pt7b);
  clear_part_screen(80, 50, 60, 110);
  m_screen.drawString(String(counter.current_minutes), 80, 50);

  m_screen.drawString(String(counter.sum_minutes), 80, 110);
}

String format_two_digits(uint8_t number)
{
  return (number < 10) ? "0" + String(number) : String(number);
}
void print_date(DateTime date)
{
  uint8_t hour = date.hour();
  uint8_t minute = date.minute();
  uint8_t day = date.day();
  uint8_t month = date.month();

  String formattedHour = format_two_digits(hour);
  String formattedMinute = format_two_digits(minute);
  String formattedDay = format_two_digits(day);
  String formattedMonth = format_two_digits(month);

  String text = formattedHour + ":" + formattedMinute;

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  clear_part_screen(60, 185, 110, 110);

  m_screen.drawString(text, 60, 185);

  text = formattedDay + "." + formattedMonth;
  m_screen.drawString(text, 60, 210);
}
void refresh_left_side(spend_time& counter)
{
  clear_part_screen(0, 0, 180, 240);
  print_date(rtc.now());
  print_time(counter);
}
void check_button()
{
  static unsigned long last_button_press_time = 0;
  unsigned long current_time = millis();

  if (current_time - last_button_press_time < debounce_delay)
  {
    return;
  }

  if (digitalRead(WIO_5S_UP) == LOW || digitalRead(WIO_5S_DOWN) == LOW || digitalRead(WIO_5S_PRESS) == LOW ||
      digitalRead(WIO_KEY_C) == LOW || digitalRead(WIO_KEY_B) == LOW || digitalRead(WIO_KEY_A) == LOW)
  {
    last_button_press_time = current_time;
  }

  if (digitalRead(WIO_5S_UP) == LOW)
  {
    auto& counter = times[m_mode];
    if (counter.check_min_position())
    {
      counter.actual_position--;
      print_side_menu(counter);
      running = false;
      counter.reset_current_time();
      refresh_left_side(counter.get_current_counter());
    }
  }
  else if (digitalRead(WIO_5S_DOWN) == LOW)
  {
    auto& counter = times[m_mode];
    if (counter.check_max_position())
    {
      counter.actual_position++;
      print_side_menu(counter);
      running = false;
      counter.reset_current_time();
      refresh_left_side(counter.get_current_counter());
    }
  }
  else if (digitalRead(WIO_5S_PRESS) == LOW)
  {
    auto& counter = times[m_mode];
    counter.get_current_counter().start_time = rtc.now();
    running = !running;
    if (!running)
    {
      counter.reset_current_time();
    }
    refresh_left_side(counter.get_current_counter());
  }
  else if (digitalRead(WIO_KEY_C) == LOW)
  {
    m_mode = Mode::chill;
    auto& counter = times[m_mode];
    print_side_menu(counter);
  }
  else if (digitalRead(WIO_KEY_B) == LOW)
  {
    auto& old_counter = times[m_mode];
    running = false;
    old_counter.reset_current_time();
    refresh_left_side(old_counter.get_current_counter());

    m_mode = Mode::work;
    auto& counter = times[m_mode];
    print_side_menu(counter);
  }
  else if (digitalRead(WIO_KEY_A) == LOW)
  {
    auto& old_counter = times[m_mode];
    running = false;
    old_counter.reset_current_time();
    refresh_left_side(old_counter.get_current_counter());

    m_mode = Mode::meeting;
    auto& counter = times[m_mode];
    print_side_menu(counter);
  }
}

bool check_date(DateTime date)
{
  auto now = rtc.now();
  if (now == date)
  {
    return false;
    counters_work.clear_sum_times();
    counters_chill.clear_sum_times();
    counters_meetings.clear_sum_times();
  }
  m_date = now;
  return true;
}

uint16_t timespan_to_minutes(const TimeSpan& span)
{
  return static_cast<uint16_t>(span.totalseconds() / 60);
}
void setup()
{
  Serial.begin(115200);
  rtc.begin();
  DateTime now = DateTime(F(__DATE__), F(__TIME__));
  Serial.println("adjust time!");
  rtc.adjust(now);

  m_date = rtc.now();

  pinMode(WIO_5S_UP, INPUT);
  pinMode(WIO_5S_DOWN, INPUT);
  pinMode(WIO_5S_LEFT, INPUT);
  pinMode(WIO_5S_RIGHT, INPUT);
  pinMode(WIO_5S_PRESS, INPUT);
  pinMode(WIO_KEY_A, INPUT);
  pinMode(WIO_KEY_B, INPUT);
  pinMode(WIO_KEY_C, INPUT);

  m_screen.begin();
  m_screen.setRotation(3);
  m_screen.setTextColor(TFT_BLACK);
  m_screen.fillScreen(TFT_WHITE);
  print_date(m_date);

  counters_work.add_counter({spend_time("Praca")});
  counters_work.add_counter({spend_time("Projekt")});
  counters_work.add_counter({spend_time("Blog")});
  counters_work.add_counter({spend_time("Dom")});
  counters_work.add_counter({spend_time("Planowanie")});
  counters_work.add_counter({spend_time("Nauka")});

  counters_meetings.add_counter({spend_time("Praca")});
  counters_meetings.add_counter({spend_time("Blogowe")});
  counters_meetings.add_counter({spend_time("Projekty")});
  counters_meetings.add_counter({spend_time("Dom")});

  counters_chill.add_counter({spend_time("YT")});
  counters_chill.add_counter({spend_time("Gry")});
  counters_chill.add_counter({spend_time("IG")});
  counters_chill.add_counter({spend_time("Offline")});

  times[Mode::work] = counters_work;
  times[Mode::meeting] = counters_meetings;
  times[Mode::chill] = counters_chill;

  auto counter = times[m_mode];
  print_side_menu(counter);
  print_time(counter.get_current_counter());
}

void loop()
{
  check_button();

  unsigned long loop_time = millis();
  if (loop_time - last_loop_time > checkt_time)
  {
    last_loop_time = loop_time;
    if (running)
    {
      DateTime now = rtc.now();
      auto& counter = times[m_mode];
      counter.update_current_time(now);
      print_time(counter.get_current_counter());
    }
    m_date = rtc.now();
    print_date(m_date);
  }
}
