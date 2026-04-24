#include "IRCommandReader.h"
#include "SerialReader.h"
#include <U8g2lib.h>
#include <OneButton.h>
#include "LEDMatrix.h" 

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

int pinIR = 12;
int pinEmergencyButton = 2;
int command = 0;

OneButton emergency(pinEmergencyButton, true, true);

LEDMatrix ledMatrix(&u8g2);
IRCommandReader commandReader;
SerialReader serialReader;

void setup() {
  Serial.begin(9600);
  commandReader.init(pinIR);
  ledMatrix.init();
  emergency.attachClick(emergencyClick);
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
}

void emergencyClick(){
  LEDMatrixModes mode = ledMatrix.getMode();

  if (mode == EMERGENCY){
    ledMatrix.setMode(CLOSE);
  }else{
    ledMatrix.setMode(EMERGENCY);
  }
}