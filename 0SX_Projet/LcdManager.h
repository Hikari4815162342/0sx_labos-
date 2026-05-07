#pragma once

#include <Arduino.h>
#include <LCD_I2C.h>
#include "Door.h"

enum LcdScreens {
  SCREEN_1,
  EMERGENCY_SCREEN
};

class LcdManager{
  private:
    const int ROWS_LCD = 2;
    const int COLS_LCD = 16;
    LCD_I2C* lcd;
    Door* door;
    LcdScreens currentScreen;
    LcdScreens lastScreen;
    void printLcd(String lcdMsg);
    void screen1();
    void emergencyScreen();
  public:
    LcdManager(LCD_I2C* lcd, Door* door);
    void update();
    void setScreen(LcdScreens screen);
};