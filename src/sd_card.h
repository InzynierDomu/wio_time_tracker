#pragma once

#include <SPI.h>
#include <Seeed_FS.h>
#include <vector>
// #include <string>
#include "SD/Seeed_SD.h"

#include <functional>

class sd_card
{
  public:
  sd_card();
  ~sd_card();

  void init();
  void save();
  bool load_counters_tree(const String& fileName, const std::function<void(const String&)>& processLine);

  private:
  File myFile;
};