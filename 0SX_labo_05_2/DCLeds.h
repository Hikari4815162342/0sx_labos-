#pragma once
#include "LED.h"
#include "MyDCMotor.h"
#include <Arduino.h>

class DCLeds{
  private:
    LED* activeLed;
    LED* emergencyLed;
    DCMotor* dc;
  public:
    DCLeds(LED* activeLed, LED* emergencyLed, DCMotor* dc);
    void begin();
    void update();
};