#include <HCSR04.h>
#include <Servo.h>
#include <OneButton.h>
#include <LCD_I2C.h>

#define TRIGGER_PIN 6
#define ECHO_PIN 7
const int PIN_OPEN_BTN = 2;
const int PIN_STOP_BTN = 3;
const int PIN_SERVO = 9;

Servo door;
OneButton openBtn = OneButton(PIN_OPEN_BTN, true, true);
OneButton stopBtn = OneButton(PIN_STOP_BTN, true, true);
LCD_I2C lcd(0x27, 16, 2);
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);

enum ServoState {OPEN, CLOSE, WAIT, STOP};
ServoState currentDoorState = STOP;
int angle = 10;
int step = 1;
unsigned long currentTime = 0;
unsigned long lastTime = 0;
int interval = 20;
int waitTime = 10000;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  door.attach(PIN_SERVO);
  door.detach();

  // openBtn.attachClick(onClickOpen);
  // stopBtn.attachClick(onClickStop);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() { 
  openBtn.tick();
  stopBtn.tick();
  updateDoor();
}

void updateDoor(){
  float distance = hc.dist();
  currentTime = millis();

  if (distance < 5){
    currentDoorState = OPEN;
  }

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    
  }
}

