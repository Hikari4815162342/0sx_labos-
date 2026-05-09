#pragma once
#include <Arduino.h>

enum DCState{
  FORWARD,
  BACKWARD,
  OFF,
  EMERGENCY_DC
};

class DCMotor{
  public:
    DCMotor(int input1, int input2);
    DCState getState() const;
    int getSpeed() const;
    int getMaxSpeed() const;
    int getMinSpeed() const;
    void setSpeed(int speed);
    void setState(DCState currentState);
    void updateDCMotor();
    bool isActive();
  private:
    int input1;
    int input2;
    int speed;
    const int MAX_SPEED = 255;
    const int MIN_SPEED = 60;
    DCState currentState;
    void forward();
    void backward();
    void off();
};