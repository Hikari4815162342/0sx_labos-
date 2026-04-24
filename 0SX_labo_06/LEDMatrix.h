#pragma once

#include <Arduino.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>

enum LEDMatrixModes{
  DISCOUNT,
  NORMAL,
  ERROR,
  CLOSE,
  EMERGENCY
};

class LEDMatrix{
  private:
    U8G2* u8g2;
    LEDMatrixModes mode;
    static const uint8_t bitmapSmiley[];
    char* message = "SPECIAL";
    uint16_t positionBits;
    uint16_t lengthBits;
    const uint8_t displaySize = 8;
    int fontSize = 4;
    const int BUFFER_SIZE = 12;
    void open();
    void off();
    void writeMessageAtPosition();
    void discount();
    void normal();
    void error();
    void close();
    void emergency();
  public:
    LEDMatrix(U8G2* ledMatrix);
    void init();
    void setMode(LEDMatrixModes mode);
    LEDMatrixModes getMode() const;
    void treatCommand(int command);
    void update();
};

