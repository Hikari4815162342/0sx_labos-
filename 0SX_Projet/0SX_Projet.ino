#include <HCSR04.h>
#include <Servo.h>
#include "Door.h"
#include "LcdManager.h"
#include <OneButton.h>
#include <LCD_I2C.h>

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
LcdManager screen(&lcd, &door);


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  door.init();

  openBtn.attachClick(onClickOpenDoor);
  emergencyBtn.attachClick(onClickEmergency);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

 void loop() { 
  openBtn.tick();
  emergencyBtn.tick();
  DoorState state = door.getState();

  if (state != EMERGENCY){
    door.readDistance();
  }

  door.update();
  screen.update();

  static unsigned long l = 0;
  if (millis() - l >= 2000){
    l = millis();
    Serial.println(door.getClients());
  }
}


void onClickOpenDoor(){
  DoorState state = door.getState();
  if (state == CLOSED || state == CLOSE){
    door.setState(OPEN);
  }
}

void onClickEmergency(){
  DoorState state = door.getState();
  if (state == EMERGENCY){
    door.setState(CLOSE);
    screen.setScreen(SCREEN_1);
  }else{
    door.setState(EMERGENCY);
    screen.setScreen(EMERGENCY_SCREEN);
  }
}


