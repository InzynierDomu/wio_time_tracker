#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "TFT_eSPI.h"

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

struct spend_time
{
  spend_time(String _name)
  : current_minutes(0)
  , sum_minutes(0)
  , name(_name)
  {}
  uint16_t current_minutes;
  uint16_t sum_minutes;
  String name;
};

RTC_SAMD51 rtc;
TFT_eSPI m_screen; ///< TFT screen 320x240

DateTime startTime;
bool running = false;
uint16_t checkt_time = 30000;
DateTime m_date;
unsigned long last_loop_time = millis();

Mode m_mode = Mode::chill;
struct time_type
{
  std::vector<spend_time> counters;
  uint8_t actual_position;

  String get_name(uint8_t position)
  {
    return counters[position].name;
  }

  uint8_t get_size()
  {
    return counters.size();
  }

  void add_counter(spend_time counter)
  {
    counters.push_back(counter);
  }

  bool check_min_position()
  {
    if (actual_position > 0)
    {
      return false;
    }
    return true;
  }

  bool check_max_position()
  {
    if (actual_position < counters.size())
    {
      return false;
    }
    return true;
  }
};

time_type counters_work;
time_type counters_meetings;
time_type counters_chill;

void print_list_counters(time_type& counters)
{
  uint8_t posY = 10;
  for (size_t i = 0; i < counters.get_size(); i++)
  {
    m_screen.drawString(counters.get_name(i), 185, posY);
    posY += 26;
  }
}
void print_side_menu(Mode mode, uint8_t position = 0)
{
  m_screen.fillRect(180, 0, 160, 240, TFT_LIGHTGREY);

  int posY = 8 + (position * 26);
  m_screen.fillRect(180, posY, 160, 26, TFT_DARKGREY);

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  switch (mode)
  {
    case Mode::chill:
      print_list_counters(counters_chill);
      break;
    case Mode::work:
      print_list_counters(counters_work);
      break;
    case Mode::meeting:
      print_list_counters(counters_meetings);
      break;
    default:
      break;
  }
}
void check_button()
{
  if (digitalRead(WIO_5S_DOWN) == LOW)
  {}
  else if (digitalRead(WIO_5S_UP) == LOW)
  {
    // (m_controller->*m_callback)(Cursor_move::up);
  }
  else if (digitalRead(WIO_5S_RIGHT) == LOW)
  {
    // (m_controller->*m_callback)(Cursor_move::right);
  }
  else if (digitalRead(WIO_5S_LEFT) == LOW)
  {
    // (m_controller->*m_callback)(Cursor_move::left);
  }
  else if (digitalRead(WIO_5S_PRESS) == LOW)
  {
    // (m_controller->*m_callback)(Cursor_move::center);
  }
  else if (digitalRead(WIO_KEY_C) == LOW)
  {
    m_mode = Mode::work;
    print_side_menu(m_mode);
  }
  else if (digitalRead(WIO_KEY_B) == LOW)
  {
    m_mode = Mode::meeting;
    print_side_menu(m_mode);
  }
  else if (digitalRead(WIO_KEY_A) == LOW)
  {
    m_mode = Mode::chill;
    print_side_menu(m_mode);
  }
}

void clear_part_screen(const uint16_t position_x, const uint16_t position_y, const uint16_t width, const uint16_t height)
{
  m_screen.fillRect(position_x, position_y, width, height, TFT_WHITE);
}

void print_time(TimeSpan time)
{
  String text = String(time.minutes());
  m_screen.setFreeFont(&FreeSansBold24pt7b);
  clear_part_screen(80, 50, 60, 80);
  m_screen.drawString(text, 80, 50);

  m_screen.drawString(text, 80, 110);
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

bool check_date(DateTime date)
{
  auto now = rtc.now();
  if (now == date)
  {
    return false;
  }
  m_date = now;
  return true;
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
  print_time(0);

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

  print_side_menu(m_mode);
}

void loop()
{
  // if (digitalRead(WIO_5S_PRESS) == LOW)
  // {
  //   startTime = rtc.now();
  //   running = true;
  // }
  check_button();

  unsigned long loop_time = millis();
  if (loop_time - last_loop_time > checkt_time)
  {
    last_loop_time = loop_time;
    TimeSpan span = 0;
    if (running)
    {
      DateTime now = rtc.now();
      span = now - startTime;
      print_time(span);
    }
    m_date = rtc.now();
    print_date(m_date);
  }
}
