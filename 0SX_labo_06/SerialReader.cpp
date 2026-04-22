#include "SerialReader.h"

SerialReader::SerialReader(){}

int SerialReader::getCommand(){
  if (Serial.available() > 0){
    char c = Serial.read();
    if (c >= '1' && c <= '4'){
      int command = c - '0';
      Serial.println(command);
      return command;
    }else{
      Serial.println("Commande invalide");
    }
  }
}