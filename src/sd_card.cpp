#include "sd_card.h"

sd_card::sd_card() {}

sd_card::~sd_card() {}

void sd_card::init()
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void sd_card::save()
{
  Serial.println("saving");
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile)
  {
    myFile.println("testing 1, 2, 3.");
    myFile.close();
    Serial.println("saved");
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