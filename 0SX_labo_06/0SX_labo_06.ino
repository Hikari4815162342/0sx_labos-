#include "IRCommandReader.h"
#include "SerialReader.h"
#include <U8g2lib.h>
#include <OneButton.h>
#include "LEDMatrix.h" 
#include "LcdManager.h"
#include <LCD_I2C.h>

#define CLK_PIN  30
#define DIN_PIN  34
#define CS_PIN   32

U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2(
  U8G2_R0,
  CLK_PIN,
  DIN_PIN,
  CS_PIN,
  U8X8_PIN_NONE,
  U8X8_PIN_NONE
);

unsigned long currentTime = 0;
unsigned long lastTime = 0;
int interval = 500;

int pinIR = 2;
int pinEmergencyButton = 12;
int command = 0;

LCD_I2C lcd(0x27, 16, 2);
OneButton emergency(pinEmergencyButton, true, true);

LEDMatrix ledMatrix(&u8g2);
LcdManager screen(&lcd, &ledMatrix);
IRCommandReader commandReader;
SerialReader serialReader;

void setup() {
  Serial.begin(9600);
  commandReader.init(pinIR);
  ledMatrix.init();
  emergency.attachClick(emergencyClick);
  lcd.begin();
  lcd.backlight();
}

void loop() {
  currentTime = millis();
  emergency.tick();
  LEDMatrixModes mode = ledMatrix.getMode();
  
  if (mode != EMERGENCY){
    
    if (commandReader.commandReceived()){
      command = commandReader.getCommand();
    }else if (serialReader.commandReceived()){
      command = serialReader.getCommand();
    }

     ledMatrix.treatCommand(command);
  }

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    u8g2.clearBuffer();
    ledMatrix.update();
    u8g2.sendBuffer();
  }

  screen.update();
}

void emergencyClick(){
  LEDMatrixModes mode = ledMatrix.getMode();

  if (mode == EMERGENCY){
    command = 0;
    ledMatrix.setMode(CLOSE);
    screen.setScreen(SCREEN_1);
  }else{
    ledMatrix.setMode(EMERGENCY);
    screen.setScreen(EMERGENCY_SCREEN);
  }
}