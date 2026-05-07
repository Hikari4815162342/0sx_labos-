#pragma once

#include <Arduino.h>
#include <HCSR04.h>
#include <Servo.h>
#include <Wire.h>

enum DoorState{
  OPEN,
  CLOSE,
  WAIT,
  CLOSED,
  EMERGENCY
};

class Door {
  private:
    Servo* door;
    HCSR04* hc;
    DoorState currentDoorState;
    int clients;
    int pinServo;
    const int MAX_ANGLE = 170;
    const int MIN_ANGLE = 10;
    int angle;
    float distance;
    unsigned long lastDistanceReading = 0;
    const int DIST_THRESHOLD = 5;
    unsigned long currentTime;
    const int interval = 10;
    const int readingDelay = 100;
    const int WAIT_TIME = 10000;
    bool openFirstTime = true;
    bool waitFirstTime = true;
    void open();
    void close();
    void closed();
    void wait();
    void emergency();
  public:
    Door(Servo* door, HCSR04* hc, const int PIN_SERVO);
    DoorState getState() const;
    void setState(DoorState state);
    int getAngle() const;
    int getClients() const;
    void init();
    void readDistance();
    void update();
};