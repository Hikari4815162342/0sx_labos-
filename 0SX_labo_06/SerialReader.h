#pragma once

#include <Arduino.h>

class SerialReader{
  public:
    SerialReader();
    int getCommand();
};