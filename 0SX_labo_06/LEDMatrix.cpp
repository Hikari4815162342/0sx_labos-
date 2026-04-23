#include "LEDMatrix.h"

LEDMatrix::LEDMatrix(U8G2* ledMatrix){
  u8g2 = ledMatrix;
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
  switch(command){
    case 1: mode = DISCOUNT; break;
    case 2: mode = NORMAL; break;
    case 3: mode = ERROR; break;
    case 4: mode = CLOSE; break;
    default: break;
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
    case CLOSE: 
      close(); 
      break;
    case EMERGENCY: 
      emergency(); 
      break;
    default: 
      break;
  }
}

void LEDMatrix::writeMessageAtPosition(){
  char buffer[this->BUFFER_SIZE];               
  uint16_t startCharacter = this->positionBits / this->fontSize; 
  uint16_t i;

  // Copier une portion du texte dans le tampon
  for (i = 0; i < this->BUFFER_SIZE - 1; i++) {
    buffer[i] = this->message[startCharacter + i];
    if (this->message[startCharacter + i] == '\0')
      break;
  }

  buffer[this->BUFFER_SIZE - 1] = '\0'; // S'assurer que le tampon est bien terminé

  // Afficher le texte en tenant compte du décalage de bits (scroll horizontal)
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

void LEDMatrix::normal(){

}

void LEDMatrix::error(){

}

void LEDMatrix::close(){

}

void LEDMatrix::emergency(){

}