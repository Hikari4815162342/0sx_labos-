#include "SerialReader.h"

SerialReader::SerialReader(){}

int SerialReader::getCommand(){
  if (Serial.available() > 0){
    char c = Serial.read();
    if (c >= '1' && c <= '4'){
      int command = c - '0';
      return command;
    }else{
      return 0;
    }
  }
}

bool SerialReader::commandReceived(){
  if (Serial.available() > 0){
    return true;
  }else if (Serial.available() == 0){
    return false;
  }
}