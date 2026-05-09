#include "LcdManager.h"

LcdManager::LcdManager(LCD_I2C* lcd, Door* door, DCMotor* dc){
  this->lcd = lcd;
  this->door = door;
  this->dc = dc;
  this->currentScreen = SCREEN_1;
  this->lastScreen = SCREEN_1;
}

void LcdManager::setScreen(LcdScreens screen){ this->currentScreen = screen; }
LcdScreens LcdManager::getLastScreen() const { return this->lastScreen; }

void LcdManager::printLcd(String lcdMsg){
  while(lcdMsg.length() < COLS_LCD){
    lcdMsg += " ";
  }
  lcd->print(lcdMsg);
}

void LcdManager::update(){
  this->currentTime = millis();
  if (this->currentScreen != this->lastScreen){
    lcd->clear();
    this->lastScreen = this->currentScreen;
  }

  switch (this->currentScreen){
    case SCREEN_1: screen1(); break;
    case SCREEN_2: screen2(); break;
    case SCREEN_3: screen3(); break;
    case EMERGENCY_SCREEN: emergencyScreen(); break;
  }
}

void LcdManager::screen1(){
  
DoorState state = door->getState();
  lcd->setCursor(0, 0);

  String currentState = "";
  switch (state){
    case OPEN:
      currentState = "Ouverture...";
      break;
    case WAIT:
      currentState = "Attente...";
      break;
    case CLOSE:
      currentState = "Fermeture...";
      break;
    case CLOSED:
      currentState = "Porte fermee";
      break;
    }

  printLcd(currentState);

  if (this->currentTime - lastScreenTime >= SCREEN_INTERVAL){
    lastScreenTime = this->currentTime;
    currentScreen = SCREEN_2;
  }
}

void LcdManager::screen2(){
  lcd->setCursor(0,0);
  
  String active = "";
  if (dc->isActive()){
    active = "OUI";
  }else{
    active = "NON";
  }
  printLcd("Actif : " + active);

  lcd->setCursor(0,1);
  
  DCState state = dc->getState();
  String direction = "";
  if (state == FORWARD){
    direction = "AVANT";
  }else if (state == BACKWARD){
    direction = "ARRIERE";
  }else if (state == OFF){
    direction = "ARRET";
  }
  printLcd("Sens : " + direction);

  if (this->currentTime - lastScreenTime >= SCREEN_INTERVAL){
    lastScreenTime = this->currentTime;
    currentScreen = SCREEN_3;
  }
}

void LcdManager::screen3(){
  lcd->setCursor(0,0);

  printLcd("Vitesse : " + String(dc->getSpeed()));

  if (this->currentTime - lastScreenTime >= SCREEN_INTERVAL){
    lastScreenTime = this->currentTime;
    currentScreen = SCREEN_1;
  }
}

void LcdManager::emergencyScreen(){
  lcd->setCursor(0, 0);
  printLcd("URGENCE");
}