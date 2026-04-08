#include "MyDCMotor.h"
#include <LCD_I2C>
#include <OneButton.h>

int input1 = 2;
int input2 = 3;
int speed = 60;

DCMotor myDC(input1, input2);

int COLS_LCD = 16;
int ROWS_LCD = 2;

LCD_I2C lcd(0x27, COLS_LCD, ROWS_LCD); 

int btnPin = 4;
int activeLedPin = 12;
int emergencyLedPin = 13;

OneButton btn(btnPin, true, true);

void setup() {
  lcd.begin();
  lcd.backlight();
  
  pinMode(activeLedPin, OUTPUT);
  pinMode(emergencyLedPin, OUTPUT);
  
  btn.attachClick(click);
}

void loop() {
  btn.tick();

}
