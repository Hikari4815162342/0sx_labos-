#include "MyDCMotor.h"
#include <LCD_I2C.h>
#include <OneButton.h>

int input1 = 44;
int input2 = 45;
const int MAX_SPEED = 255;
const int MIN_SPEED = 60;

DCMotor myDC(input1, input2);

int COLS_LCD = 16;
int ROWS_LCD = 2;

LCD_I2C lcd(0x27, COLS_LCD, ROWS_LCD); 
enum LcdScreens{
    EMERGENCY_SCREEN,
    SCREEN_1,
    SCREEN_2
};
LcdScreens currentScreen = SCREEN_1;
LcdScreens lastScreen = SCREEN_1;
int screenInterval = 5000;
unsigned long lastScreenTime = 0;

int btnPin = 2;
int activeLedPin = 12;
int emergencyLedPin = 13;

OneButton btn(btnPin, true, true);

int xAxis = A0;
int yAxis = A1;

unsigned long currentTime = 0;
unsigned long lastTime = 0;
int interval = 100;

bool buttonPressed = false;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  
  pinMode(activeLedPin, OUTPUT);
  pinMode(emergencyLedPin, OUTPUT);
  
  btn.attachClick(emergencyClick);
}

void loop() {
  currentTime = millis();
  btn.tick();
  if (myDC.getState() != EMERGENCY){
    readJoystick();
  }
  handleLeds();
  updateLcd();
}

void readJoystick(){
  int xValue = analogRead(xAxis);
  int yValue = analogRead(yAxis);
  DCState state = OFF;
  static int speed = 60;
  static unsigned long lastSTime = 0;
  
  if (yValue < 400){
    state = BACKWARD;
  }else if (yValue > 600){
    state = FORWARD;
  }else if (yValue >= 400 && yValue <= 600){
    state = OFF;
  }

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    if (xValue < 400){
      speed += 2;
    }else if (xValue > 600){
      speed -= 2;
    }
  }
  
  if (speed >= MAX_SPEED){
    speed = 255;
  }else if (speed <= MIN_SPEED){
    speed = 60;
  }
  
  myDC.setState(state);
  myDC.setSpeed(speed);
  myDC.updateDCMotor();
}

void handleLeds(){
  DCState state = myDC.getState();
  if (state != EMERGENCY){
    digitalWrite(emergencyLedPin, LOW);
    if (state == FORWARD || state == BACKWARD){
      digitalWrite(activeLedPin, HIGH);
    }else{
      digitalWrite(activeLedPin, LOW);
    }  
  }else if (state == EMERGENCY){
    digitalWrite(activeLedPin, LOW);
    digitalWrite(emergencyLedPin, HIGH);
  }
}

void emergencyClick(){
  if (myDC.getState() == EMERGENCY){
    myDC.setState(OFF);
  }else{
    myDC.setState(EMERGENCY);
  }
  myDC.updateDCMotor();
  buttonPressed = true;
}

void printLcd(String lcdMsg){
  while(lcdMsg.length() < COLS_LCD){
    lcdMsg += " ";
  }
  lcd.print(lcdMsg);
}

void updateLcd(){
  
  if (currentScreen != lastScreen){
    lcd.clear();
    lastScreen = currentScreen;
  }

  switch (currentScreen){
    case SCREEN_1:
      screen1();
      break;
    case SCREEN_2:
      screen2();
      break;
    case EMERGENCY_SCREEN:
      emergencyScreen();
      break;
  }
}

void screen1(){
  lcd.setCursor(0,0);
  
  if (myDC.isActive()){
    printLcd(String("Actif : ") + "OUI");
  }else{
    printLcd(String("Actif : ")  + "NON");
  }

  lcd.setCursor(0,1);
  DCState state = myDC.getState();
  String sens = "";
  if (state == FORWARD){
    sens = "AVANT";
  }else if (state == BACKWARD){
    sens = "ARRIERE";
  }else if (state == OFF){
    sens = "ARRET";
  }
  printLcd("Sens : " + sens);

  bool transition = buttonPressed;
  if (transition){
    buttonPressed = false;
    currentScreen = EMERGENCY_SCREEN;
  }

  if (currentTime - lastScreenTime >= screenInterval){
    lastScreenTime = currentTime;
    currentScreen = SCREEN_2;
  }
}

void screen2(){
  lcd.setCursor(0,0);

  printLcd("Vitesse : " + String(myDC.getSpeed()));

  bool transition = buttonPressed;
  if (transition){
    buttonPressed = false;
    currentScreen = EMERGENCY_SCREEN;
  }

  if (currentTime - lastScreenTime >= screenInterval){
    lastScreenTime = currentTime;
    currentScreen = SCREEN_1;
  }
}

void emergencyScreen(){
  lcd.setCursor(0,0);
  
  printLcd("URGENCE");

  bool transition = buttonPressed;
  if (transition){
    buttonPressed = false;
    currentScreen = SCREEN_1;
  }
}

