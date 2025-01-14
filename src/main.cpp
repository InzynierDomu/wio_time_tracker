#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "counters_generator.h"
#include "counters_saver.h"
#include "gui.h"
#include "sd_card.h"
#include "time_category.h"

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
// TFT_eSPI m_screen; ///< TFT screen 320x240
gui m_gui;
sd_card sd;

bool running = false;
uint16_t checkt_time = 30000;
DateTime m_date;
unsigned long last_loop_time = millis();

const unsigned long debounce_delay = 500;

Mode m_mode = Mode::chill;

time_category counters_work;
time_category counters_meetings;
time_category counters_chill;

std::map<Mode, time_category> times;

void parse_mode(Mode mode, String name)
{
  counters_saver saver(times[mode], name);
  auto parser = [&](String& json) { saver.parse(json); };
  sd.save_counters_value(parser);
}

void save_counters_sd()
{
  sd.clear_file();
  sd.add_file_border();
  parse_mode(Mode::work, "work");
  parse_mode(Mode::meeting, "meeting");
  parse_mode(Mode::chill, "chill");
  sd.add_file_border();
}

void check_button()
{
  static unsigned long last_button_press_time = 0;
  unsigned long current_time = millis();

  if (current_time - last_button_press_time < debounce_delay)
  {
    return;
  }

  // Sprawdzenie, czy którykolwiek przycisk został naciśnięty
  if (digitalRead(WIO_5S_UP) == LOW || digitalRead(WIO_5S_DOWN) == LOW || digitalRead(WIO_5S_PRESS) == LOW ||
      digitalRead(WIO_KEY_C) == LOW || digitalRead(WIO_KEY_B) == LOW || digitalRead(WIO_KEY_A) == LOW)
  {
    last_button_press_time = current_time;
  }

  auto handle_mode_change = [&](Mode new_mode) {
    auto& old_counter = times[m_mode];
    running = false;
    old_counter.reset_current_time();
    m_gui.refresh_left_side(running, old_counter.get_current_counter(), rtc.now());

    m_mode = new_mode;
    auto& counter = times[m_mode];
    m_gui.print_side_menu(counter);
    m_gui.refresh_left_side(running, counter.get_current_counter(), rtc.now());
  };

  auto& counter = times[m_mode];
  if (digitalRead(WIO_5S_UP) == LOW)
  {
    if (counter.check_min_position())
    {
      counter.actual_position--;
      m_gui.print_side_menu(counter);
      running = false;
      counter.reset_current_time();
      m_gui.refresh_left_side(running, counter.get_current_counter(), rtc.now());
    }
  }
  else if (digitalRead(WIO_5S_DOWN) == LOW)
  {
    if (counter.check_max_position())
    {
      counter.actual_position++;
      m_gui.print_side_menu(counter);
      running = false;
      counter.reset_current_time();
      m_gui.refresh_left_side(running, counter.get_current_counter(), rtc.now());
    }
  }
  else if (digitalRead(WIO_5S_PRESS) == LOW)
  {
    counter.get_current_counter().start_time = rtc.now();
    running = !running;
    if (!running)
    {
      counter.reset_current_time();
    }
    m_gui.refresh_left_side(running, counter.get_current_counter(), rtc.now());
    save_counters_sd();
  }
  else if (digitalRead(WIO_KEY_C) == LOW)
  {
    handle_mode_change(Mode::chill);
  }
  else if (digitalRead(WIO_KEY_B) == LOW)
  {
    handle_mode_change(Mode::work);
  }
  else if (digitalRead(WIO_KEY_A) == LOW)
  {
    handle_mode_change(Mode::meeting);
  }
}

void check_date(DateTime date)
{
  auto now = rtc.now();
  if (now.day() != date.day())
  {
    m_date = now;
    counters_work.clear_sum_times();
    counters_chill.clear_sum_times();
    counters_meetings.clear_sum_times();
  }
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

  m_gui.init();
  m_gui.print_date_time(running, m_date);

  sd.init();

  counters_generator parser(counters_work, counters_meetings, counters_chill);
  if (!sd.load_counters_tree("/data.json", [&](const String& line) { parser.processLine(line); }))
  {
    // error
  }

  times[Mode::work] = counters_work;
  times[Mode::meeting] = counters_meetings;
  times[Mode::chill] = counters_chill;

  auto counter = times[m_mode];
  m_gui.print_side_menu(counter);
  m_gui.print_time(running, counter.get_current_counter());
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
      m_gui.print_time(running, counter.get_current_counter());
    }
    m_date = rtc.now();
    m_gui.print_date_time(running, m_date);
    check_date(m_date);
  }
}
