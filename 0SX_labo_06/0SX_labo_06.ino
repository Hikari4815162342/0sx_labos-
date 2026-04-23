#include "IRCommandReader.h"
#include "SerialReader.h"
#include <U8g2lib.h>
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

int pinIR = 12;
int command = 0;

LEDMatrix ledMatrix(&u8g2);
IRCommandReader commandReader;
SerialReader serialReader;

void setup() {
  Serial.begin(9600);
  commandReader.init(pinIR);
  ledMatrix.init();
}

void loop() {
  if (commandReader.commandReceived()){
    command = commandReader.getCommand();
  }else if (serialReader.commandReceived()){
    command = serialReader.getCommand();
  }
  
  ledMatrix.treatCommand(command);
  u8g2.clearBuffer();
  ledMatrix.update();
  u8g2.sendBuffer();

  delay(500);
}