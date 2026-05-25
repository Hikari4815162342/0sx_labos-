#pragma once
#include <Arduino.h>


class IRCommandReader{
  public:
    IRCommandReader();
    void init(int pin);
    bool getEvent(int& command);
};