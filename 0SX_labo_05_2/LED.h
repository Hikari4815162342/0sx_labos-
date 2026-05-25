#pragma once

#include <Arduino.h>

class LED{
  private:
    int pin;
  public:
    LED(int pin);
    void init();
    void on();
    void off();
};