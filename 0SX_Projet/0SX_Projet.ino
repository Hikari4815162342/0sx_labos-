#include <HCSR04.h>
#include <Servo.h>
#include "Door.h"
#include "LcdManager.h"
#include <OneButton.h>
#include <LCD_I2C.h>
#include "MyDCMotor.h"
#include "DCJoystick.h"
#include "DCLeds.h"
#include "LED.h"
#include "LEDMatrix.h"
#include "SerialReader.h" 
#include "IRCommandReader.h"
#include <U8g2lib.h>

//Variables et objets labo4
#define TRIGGER_PIN 7
#define ECHO_PIN 8
const int PIN_OPEN_BTN = 11;
const int PIN_EMERGENCY_BTN = 12;
const int PIN_SERVO = 6;

Servo servo;
OneButton openBtn = OneButton(PIN_OPEN_BTN, true, true);
OneButton emergencyBtn = OneButton(PIN_EMERGENCY_BTN, true, true);
LCD_I2C lcd(0x27, 16, 2);
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);
const int DIST_THRESHOLD = 5;

Door door(&servo, &hc, PIN_SERVO);

//Variables et objets labo5
int input1 = 4;
int input2 = 3;
int xAxis = A0;
int yAxis = A1;

int emergencyLedPin = 10;
int activeLedPin = 9;

DCMotor myDC(input1, input2);
DCJoystick DCJoystick(xAxis, yAxis, &myDC);
LED emergencyLed(emergencyLedPin);
LED activeLed(activeLedPin);
DCLeds DCLeds(&activeLed, &emergencyLed, &myDC);

//Variables et objet labo6
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
int command = 0;

LEDMatrix ledMatrix(&u8g2);
IRCommandReader commandReader;
SerialReader serialReader;

LcdManager screen(&lcd, &door, &myDC, &ledMatrix);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  door.init();

  openBtn.attachClick(onClickOpenDoor);
  emergencyBtn.attachClick(onClickEmergency);

  DCLeds.begin();

  commandReader.init(pinIR);
  ledMatrix.init();

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

 void loop() { 
  currentTime = millis();
  openBtn.tick();
  emergencyBtn.tick();
  DoorState doorState = door.getState();
  DCState dcState = myDC.getState();
  LEDMatrixModes mode = ledMatrix.getMode();

  if (doorState != EMERGENCY_DDOR && dcState != EMERGENCY_DC && mode != EMERGENCY_MATRIX){
    if (commandReader.commandReceived()){
      command = commandReader.getCommand();
    }else if (serialReader.commandReceived()){
      command = serialReader.getCommand();
    }

    ledMatrix.treatCommand(command);
    door.readDistance();
    DCJoystick.update();
    myDC.updateDCMotor();
  }

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    u8g2.clearBuffer();
    ledMatrix.update();
    u8g2.sendBuffer();
  }

  door.update();
  DCLeds.update();
  screen.update();
}


void onClickOpenDoor(){
  DoorState state = door.getState();
  if (state == CLOSED || state == CLOSE){
    door.setState(OPEN);
  }
}

void onClickEmergency(){
  DoorState doorState = door.getState();
  DCState dcState = myDC.getState();
  LEDMatrixModes mode = ledMatrix.getMode();

  if (doorState == EMERGENCY_DDOR && dcState == EMERGENCY_DC && mode == EMERGENCY_MATRIX){
    command = 0;
    ledMatrix.setMode(SHUT_OFF);
    door.setState(CLOSE);
    myDC.setState(OFF);
    screen.setScreen(SCREEN_1);
  }else{
    ledMatrix.setMode(EMERGENCY_MATRIX);
    door.setState(EMERGENCY_DDOR);
    myDC.setState(EMERGENCY_DC);
    screen.setScreen(EMERGENCY_SCREEN);
  }
}


