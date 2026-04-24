#include "LcdManager.h"

LcdManager::LcdManager(LCD_I2C* lcd, LEDMatrix* matrix){
  this->lcd = lcd;
  this->matrix = matrix;
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
  LEDMatrixModes mode = matrix->getMode();
  lcd->setCursor(0, 0);

  String currentMode = "";
  switch(mode){
    case DISCOUNT:
      currentMode = "Rabais";
      break;
    case NORMAL: 
      currentMode = "Normal";
      break;
    case ERROR: 
      currentMode = "Erreur"; 
      break;
    case CLOSE: 
      currentMode = "Fermer";
      break;
  }

  printLcd("Mode : " + currentMode);
}

void LcdManager::emergencyScreen(){
  lcd->setCursor(0, 0);
  printLcd("URGENCE");
}