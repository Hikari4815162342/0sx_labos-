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
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
LCD_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);
const int DIST_THRESHOLD = 5;

enum ServoState {OPEN, CLOSE, WAIT, STOP, CLOSED};
ServoState currentDoorState = CLOSED;
int angle = 10;
unsigned long currentTime = 0;
int interval = 20;
int waitTime = 10000;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  door.attach(PIN_SERVO);
  door.write(angle);

  openBtn.attachClick(onClickOpen);
  stopBtn.attachClick(onClickStop);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() { 
  currentTime = millis();
  
  openBtn.tick();
  stopBtn.tick();
  updateDoor();
}
void updateDoor(){
  static unsigned long lastDistanceReading = 0;
  static float distance = 10;
  String lcdMsg = "Porte fermee";

  if (currentTime - lastDistanceReading >= 100){
    lastDistanceReading = currentTime;
    distance = hc.dist();
  }

 if (distance <= DIST_THRESHOLD && currentDoorState != WAIT && currentDoorState != STOP){
    currentDoorState = OPEN;
  }
  
  if ((currentDoorState != STOP || currentDoorState != CLOSED || currentDoorState != WAIT) && !door.attached()){
    door.attach(PIN_SERVO);
  }

  switch (currentDoorState){
    case OPEN:
      open();
      lcdMsg = "Ouverture...";
      break;
    case WAIT:
      wait();
      lcdMsg = "Attente...";
      break;
    case CLOSE:
      close();
      lcdMsg = "Fermeture...";
      break;
    case STOP:
      lcdMsg = "ARRET D'URGENCE!";
      break;
    case CLOSED:
      break;
  }

  printLcd(lcdMsg);
}

void open(){
  static unsigned long lastOpenTime = 0;
  if (currentTime - lastOpenTime >= interval){
    lastOpenTime = currentTime;
    if (angle < 170){
      ++angle;
    }else {
      angle = 170;
      door.detach();
      currentDoorState = WAIT;
    }
    door.write(angle);
  }
}

void wait(){
  static unsigned long lastWaitTime = 0;
  
  if (lastWaitTime == 0){
    lastWaitTime = currentTime;
  }
  
  if (currentTime - lastWaitTime >= waitTime){
    lastWaitTime = 0;
    currentDoorState = CLOSE;
  }
}

void close(){
  static unsigned long lastCloseTime = 0;
  if (currentTime - lastCloseTime >= interval){
    lastCloseTime = currentTime;
    if (angle > 10){
      --angle;
    }else {
      angle = 10;
      door.detach();
      currentDoorState = CLOSED;
    }
    door.write(angle);
  }
}

void onClickOpen(){
  if (currentDoorState == CLOSED){
    currentDoorState = OPEN;
  }else if (currentDoorState == CLOSE){
    currentDoorState = OPEN;
  }
}

bool onClickStop(){
  static bool state = true;
  if (state) {
    currentDoorState = STOP;
    door.detach();
    state = false; 
  }else{
    currentDoorState = CLOSE;
    state = true;
  }
}

void printLcd(String lcdMsg){
  lcd.setCursor(0,0);
  int sizeMsg = sizeof(lcdMsg)/sizeof(lcdMsg[0]);
  lcd.print(lcdMsg);
  for (int i = sizeMsg - 1; i < LCD_COLS; i++){
    lcd.print(" ");
  }
}


