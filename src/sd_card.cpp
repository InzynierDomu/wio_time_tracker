#include "sd_card.h"

void sd_card::init()
{
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI))
  {
    // error
    return;
  }
}

void sd_card::clear_file()
{
  myFile = SD.open("/record.json", FILE_WRITE);
  if (myFile)
  {
    myFile.close();
  }
}
void sd_card::save_counters_value(const String& jsonString)
{
  myFile = SD.open("/record.json", FILE_APPEND);
  {
    myFile.println(jsonString.c_str());
    myFile.close();
  }
}

bool sd_card::load_counters_tree(const String& fileName, const std::function<void(const String&)>& processLine)
{
  File file = SD.open(fileName, FILE_READ);
  if (!file)
  {
    // error
    return false;
  }

  String line;
  while (file.available())
  {
    char c = file.read();
    if (c == '\n')
    {
      processLine(line);
      line = "";
    }
    else
    {
      line += c;
    }
  }

  if (line.length() > 0)
  {
    processLine(line);
  }

  file.close();
  return true;
}