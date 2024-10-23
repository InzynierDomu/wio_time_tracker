#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "TFT_eSPI.h"

#include <Arduino.h>
#undef min
#undef max
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
  uint16_t current_minutes;
  uint16_t sum_minutes;
  String name;
  Mode mode;
};

RTC_SAMD51 rtc;
TFT_eSPI m_screen; ///< TFT screen 320x240

DateTime startTime;
bool running = false;
uint16_t checkt_time = 30000;
DateTime m_date;
unsigned long last_loop_time = millis();

Mode m_mode = Mode::chill;

std::vector<spend_time> counters;

void check_button()
{
  if (digitalRead(WIO_5S_DOWN) == LOW)
  {
    // (m_controller->*m_callback)(Cursor_move::down);
  }
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
  else if (digitalRead(WIO_KEY_A) == LOW)
  {
    m_mode = Mode::work;
  }
  else if (digitalRead(WIO_KEY_B) == LOW)
  {
    m_mode = Mode::meeting;
  }
  else if (digitalRead(WIO_KEY_C) == LOW)
  {
    m_mode = Mode::chill;
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

void print_side_menu()
{
  m_screen.fillRect(180, 0, 160, 240, TFT_LIGHTGREY);
  m_screen.fillRect(180, 34, 160, 26, TFT_DARKGREY);

  m_screen.setFreeFont(&FreeSansBold12pt7b);

  m_screen.drawString("opcja 1", 185, 10);
  m_screen.drawString("opcja 2", 185, 36);
  m_screen.drawString("opcja 3", 185, 62);
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

  counters.push_back({30, 120, "Work", Mode::work});

  print_side_menu();
}

void loop()
{
  if (digitalRead(WIO_5S_PRESS) == LOW)
  {
    startTime = rtc.now();
    running = true;
  }

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
