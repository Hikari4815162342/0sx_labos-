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

Door door(&servo, &hc, PIN_SERVO);
LcdManager screen(&lcd, &door, &myDC);


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  door.init();

  openBtn.attachClick(onClickOpenDoor);
  emergencyBtn.attachClick(onClickEmergency);

  DCLeds.begin();

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

 void loop() { 
  openBtn.tick();
  emergencyBtn.tick();
  DoorState doorState = door.getState();
  DCState dcState = myDC.getState();

  if (doorState != EMERGENCY_DDOR && dcState != EMERGENCY_DC){
    door.readDistance();
    DCJoystick.update();
    myDC.updateDCMotor();
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
  if (doorState == EMERGENCY_DDOR && dcState == EMERGENCY_DC){
    door.setState(CLOSE);
    myDC.setState(OFF);
    screen.setScreen(SCREEN_1);
  }else{
    door.setState(EMERGENCY_DDOR);
    myDC.setState(EMERGENCY_DC);
    screen.setScreen(EMERGENCY_SCREEN);
  }
}


