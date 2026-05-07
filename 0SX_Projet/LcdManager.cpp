#include "LcdManager.h"

LcdManager::LcdManager(LCD_I2C* lcd, Door* door){
  this->lcd = lcd;
  this->door = door;
  this->currentScreen = SCREEN_1;
  this->lastScreen = SCREEN_1;
}

void LcdManager::setScreen(LcdScreens screen){ this->currentScreen = screen; }

void LcdManager::printLcd(String lcdMsg){
  while(lcdMsg.length() < COLS_LCD){
    lcdMsg += " ";
  }
  lcd->print(lcdMsg);
}

void LcdManager::update(){
  if (this->currentScreen != this->lastScreen){
    lcd->clear();
    this->lastScreen = this->currentScreen;
  }

  switch (this->currentScreen){
    case SCREEN_1: screen1(); break;
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
}

void LcdManager::emergencyScreen(){
  lcd->setCursor(0, 0);
  printLcd("URGENCE");
}