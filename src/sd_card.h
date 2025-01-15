#pragma once

#include "SD/Seeed_SD.h"

#include <SPI.h>
#include <Seeed_FS.h>
#include <functional>

class sd_card
{
  public:
  sd_card() {}

  void init();
  void set_save_data_file_name(String file_name);
  void clear_file();
  void add_file_border();
  void save_counters_value(const std::function<void(String&)>& parser);
  bool load_counters_tree(const String& fileName, const std::function<void(const String&)>& processLine);

  private:
  File myFile;
  String save_file_name;
};