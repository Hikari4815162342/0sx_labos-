#pragma once
#include <Arduino.h>

enum DCState{
  FORWARD,
  BACKWARD,
  OFF,
  EMERGENCY
};

class DCMotor{
  public:
    DCMotor(int input1, int input2);
    DCState getState() const;
    int getSpeed() const;
    void setSpeed(int speed);
    void setState(DCState currentState);
    void updateDCMotor();
    bool isActive();
  private:
    int input1;
    int input2;
    int speed;
    int maxSpeed;
    int minSpeed;
    DCState currentState;
    void forward();
    void backward();
    void off();
    void emergency();
};