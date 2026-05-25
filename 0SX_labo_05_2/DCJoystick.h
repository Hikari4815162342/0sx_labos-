#pragma once
#include "MyDCMotor.h"
#include <Arduino.h>

class DCJoystick {
  private:
    int xAxis;
    int yAxis;
    int xValue;
    int yValue;
    unsigned long currentTime = 0;
    int speedInterval = 100;
    DCMotor* dcPtr;
    int readAndGetYValue();
    int readAndGetXValue();
  public:
    DCJoystick(int xAxis, int yAxis, DCMotor* dcPtr);
    void update();
};