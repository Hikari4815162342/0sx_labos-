#pragma once

#include <Arduino.h>
#include <LCD_I2C.h>
#include "Door.h"
#include "MyDCMotor.h"
#include "LEDMatrix.h"

enum LcdScreens {
  SCREEN_1,
  SCREEN_2,
  SCREEN_3,
  SCREEN_4,
  EMERGENCY_SCREEN
};

class LcdManager{
  private:
    const int ROWS_LCD = 2;
    const int COLS_LCD = 16;
    const int SCREEN_INTERVAL = 5000;
    unsigned long lastScreenTime;
    unsigned long currentTime;
    LCD_I2C* lcd;
    Door* door;
    DCMotor* dc;
    LEDMatrix* matrix;
    LcdScreens currentScreen;
    LcdScreens lastScreen;
    void printLcd(String lcdMsg);
    void screen1();
    void screen2();
    void screen3();
    void screen4();
    void emergencyScreen();
  public:
    LcdManager(LCD_I2C* lcd, Door* door, DCMotor* dc, LEDMatrix* matrix);
    void update();
    void setScreen(LcdScreens screen);
    LcdScreens getLastScreen() const;
};