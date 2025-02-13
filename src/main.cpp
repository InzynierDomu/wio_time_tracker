#include "DateTime.h"
#include "RTC_SAMD51.h"
#include "config.h"
#include "counters_generator.h"
#include "counters_saver.h"
#include "gui.h"
#include "sd_card.h"
#include "time_category.h"
#include "time_clock.h"
#include "wifi_info.h"

#include <Arduino.h>

#undef min
#undef max
#include <map>
#include <vector>

enum class ButtonEvent
{
  None,
  Up,
  Down,
  Press,
  Left,
  Right,
  KeyA,
  KeyB,
  KeyC
};

///< modes which device possible running
enum class Mode
{
  work,
  meeting,
  chill
};

time_clock m_clock;
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
  auto parser = [&](String& line) { saver.parse(line); };
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

ButtonEvent getButtonEvent()
{
  if (digitalRead(WIO_5S_UP) == LOW)
    return ButtonEvent::Up;
  if (digitalRead(WIO_5S_DOWN) == LOW)
    return ButtonEvent::Down;
  if (digitalRead(WIO_5S_PRESS) == LOW)
    return ButtonEvent::Press;
  if (digitalRead(WIO_KEY_C) == LOW)
    return ButtonEvent::KeyC;
  if (digitalRead(WIO_KEY_B) == LOW)
    return ButtonEvent::KeyB;
  if (digitalRead(WIO_KEY_A) == LOW)
    return ButtonEvent::KeyA;
  if (digitalRead(WIO_5S_LEFT) == LOW)
    return ButtonEvent::Left;
  if (digitalRead(WIO_5S_RIGHT) == LOW)
    return ButtonEvent::Right;
  return ButtonEvent::None;
}

void refreshUI(time_category& counter)
{
  m_gui.refresh_left_side(running, counter.get_current_counter(), m_clock.get_now());
}

void processUpEvent(time_category& counter)
{
  if (running)
  {
    save_counters_sd();
  }
  if (counter.check_min_position())
  {
    counter.actual_position--;
    m_gui.print_side_menu(counter);
    running = false;
    counter.reset_current_time();
    refreshUI(counter);
  }
}

void processDownEvent(time_category& counter)
{
  if (running)
  {
    save_counters_sd();
  }
  if (counter.check_max_position())
  {
    counter.actual_position++;
    m_gui.print_side_menu(counter);
    running = false;
    counter.reset_current_time();
    refreshUI(counter);
  }
}

void processPressEvent(time_category& counter)
{
  counter.get_current_counter().start_time = m_clock.get_now();
  running = !running;
  if (!running)
  {
    save_counters_sd();
    counter.reset_current_time();
  }
  refreshUI(counter);
}

void processModeChange(Mode newMode)
{
  auto& old_counter = times[m_mode];
  running = false;
  old_counter.reset_current_time();
  refreshUI(old_counter);

  m_mode = newMode;
  auto& counter = times[m_mode];
  m_gui.print_side_menu(counter);
  refreshUI(counter);
  save_counters_sd();
}

void processLeftEvent(time_category& counter)
{
  if (running)
  {
    counter.decrese_sum_minutes(counter.actual_position);
  }
  refreshUI(counter);
}

void processRightEvent(time_category& counter)
{
  if (running)
  {
    counter.increse_sum_minutes(counter.actual_position);
  }
  refreshUI(counter);
}

void check_button()
{
  static unsigned long last_button_press_time = 0;
  unsigned long current_time = millis();

  if (current_time - last_button_press_time < debounce_delay)
  {
    return;
  }

  ButtonEvent event = getButtonEvent();
  if (event == ButtonEvent::None)
  {
    return;
  }

  last_button_press_time = current_time;

  auto& counter = times[m_mode];
  switch (event)
  {
    case ButtonEvent::Up:
      processUpEvent(counter);
      break;
    case ButtonEvent::Down:
      processDownEvent(counter);
      break;
    case ButtonEvent::Press:
      processPressEvent(counter);
      break;
    case ButtonEvent::KeyC:
      processModeChange(Mode::chill);
      break;
    case ButtonEvent::KeyB:
      processModeChange(Mode::work);
      break;
    case ButtonEvent::KeyA:
      processModeChange(Mode::meeting);
      break;
    case ButtonEvent::Left:
      processLeftEvent(counter);
      break;
    case ButtonEvent::Right:
      processRightEvent(counter);
      break;
    default:
      break;
  }
}

String cover_data_to_name(DateTime& date)
{
  String name = "/";
  name += String(date.day());
  name += "-";
  name += String(date.month());
  name += "-";
  name += String(date.year());
  name += ".md";
  return name;
}

void check_date()
{
  auto now = m_clock.get_now();
  if (now.day() != m_date.day())
  {
    save_counters_sd();
    for (auto& [mode, counter] : times)
    {
      counter.clear_sum_times();
    }

    auto& counter = times[m_mode];
    m_gui.refresh_left_side(running, counter.get_current_counter(), now);

    sd.set_save_data_file_name(cover_data_to_name(now));
    save_counters_sd();
  }
  m_date = now;
}

uint16_t timespan_to_minutes(const TimeSpan& span)
{
  return static_cast<uint16_t>(span.totalseconds() / 60);
}

void setup_buttons()
{
  pinMode(WIO_5S_UP, INPUT);
  pinMode(WIO_5S_DOWN, INPUT);
  pinMode(WIO_5S_LEFT, INPUT);
  pinMode(WIO_5S_RIGHT, INPUT);
  pinMode(WIO_5S_PRESS, INPUT);
  pinMode(WIO_KEY_A, INPUT);
  pinMode(WIO_KEY_B, INPUT);
  pinMode(WIO_KEY_C, INPUT);
}

void setup_counters()
{
  counters_generator parser(counters_work, counters_meetings, counters_chill);
  if (!sd.load_counters_tree(config::counter_list_path, [&](const String& line) { parser.processLine(line); }))
  {
    // error TBD
  }

  times[Mode::work] = counters_work;
  times[Mode::meeting] = counters_meetings;
  times[Mode::chill] = counters_chill;
}

void init_gui()
{
  m_gui.init();
  m_gui.print_welcome();
}
void setup()
{
  init_gui();

  sd.init();

  wifi_info wifi_secrets = sd.load_wifi_config(config::wifi_config_path);
  m_clock.init(wifi_secrets);
  m_date = m_clock.get_now();

  sd.set_save_data_file_name(cover_data_to_name(m_date));

  setup_buttons();

  setup_counters();

  auto counter = times[m_mode];
  m_gui.refresh_left_side(running, counter.get_current_counter(), m_date);
  m_gui.print_side_menu(counter);
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
      auto& counter = times[m_mode];
      DateTime now = m_clock.get_now();
      counter.update_current_time(now);
      m_gui.print_time(running, counter.get_current_counter());
    }
    check_date();
    m_gui.print_date_time(running, m_date);
  }
}
