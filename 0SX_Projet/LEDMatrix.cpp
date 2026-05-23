#include "LEDMatrix.h"

LEDMatrix::LEDMatrix(U8G2* ledMatrix){
  u8g2 = ledMatrix;
  this->mode = SHUT_OFF;
}

void LEDMatrix::open(){
  for(int i = 0; i <= 8; i++){
    for (int j = 0; j <= 8; j++){
      u8g2->drawPixel(i, j);
    }
  }
}

void LEDMatrix::off(){
  u8g2->clearBuffer();
  u8g2->sendBuffer();
}

LEDMatrixModes LEDMatrix::getMode() const { return this->mode; }
void LEDMatrix::setMode(LEDMatrixModes mode){ this->mode = mode; }

void LEDMatrix::init(){
  u8g2->begin();
  u8g2->setContrast(10);
  
  u8g2->setFont(u8g2_font_4x6_tr);
  u8g2->setDisplayRotation(U8G2_R0);            
  this->positionBits = 0;
  this->lengthBits = strlen(this->message) * this->fontSize + this->displaySize; 
}

void LEDMatrix::treatCommand(int command){
  LEDMatrixModes mode = -1;
  LEDMatrixModes lastMode = this->mode;

  switch(command){
    case 1: mode = DISCOUNT; break;
    case 2: mode = NORMAL; break;
    case 3: mode = ERROR; break;
    case 4: mode = SHUT_OFF; break;
    default: mode = lastMode; break;
  }
  setMode(mode);
}

void LEDMatrix::update(){
  switch(this->mode){
    case DISCOUNT:
      discount(); 
      break;
    case NORMAL: 
      normal(); 
      break;
    case ERROR: 
      error(); 
      break;
    case SHUT_OFF: 
      shutOff(); 
      break;
    case EMERGENCY_MATRIX: 
      emergencyMatrix(); 
      break;
  }
}

void LEDMatrix::writeMessageAtPosition(){
  char buffer[this->BUFFER_SIZE];               
  uint16_t startCharacter = this->positionBits / this->fontSize; 
  uint16_t i;

  for (i = 0; i < this->BUFFER_SIZE - 1; i++) {
    buffer[i] = this->message[startCharacter + i];
    if (this->message[startCharacter + i] == '\0')
      break;
  }

  buffer[this->BUFFER_SIZE - 1] = '\0';

  u8g2->drawStr(-(this->positionBits & (this->fontSize - 1)), 7, buffer);
}

void LEDMatrix::discount(){
  u8g2->clearBuffer();                               
  writeMessageAtPosition();   
  u8g2->sendBuffer();                                

  this->positionBits++;                                       

  if ( this->positionBits >= this->lengthBits) {
     this->positionBits = 0;                                  
  }  
}

const uint8_t LEDMatrix::bitmapSmiley[] PROGMEM = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};


void LEDMatrix::normal(){
  u8g2->drawXBMP(0, 0, 8, 8, bitmapSmiley);
}

void LEDMatrix::error(){
  static unsigned long currentTime = 0;
  static unsigned long lastTime = 0;
  int interval = 1000;

  currentTime = millis();

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    open();
  }else{
    off();
  }
}

void LEDMatrix::shutOff(){
  off();
}

void LEDMatrix::emergencyMatrix(){
  u8g2->drawLine(0, 0, 7, 7);
  u8g2->drawLine(7, 0, 0, 7);
}