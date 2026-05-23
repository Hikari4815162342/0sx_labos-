#include <Arduino.h>
#include <IRremote.hpp>
#include "IRCommandReader.h"

IRCommandReader::IRCommandReader(){};

void IRCommandReader::init(int pin){
  IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);
}

bool IRCommandReader::commandReceived(){
  if (IrReceiver.decode()){
    return true;
  }
  return false;
}

int IRCommandReader::getCommand(){
  if (IrReceiver.decode()) {
      int command = IrReceiver.decodedIRData.command;

      if (IrReceiver.decodedIRData.flags && IRDATA_FLAGS_IS_REPEAT || command == 0) {
        IrReceiver.resume();
        return;
      }
  
    switch (command){
        case 12: command = 1; break;
        case 24: command = 2; break;
        case 94: command = 3; break;
        case 8: command = 4; break;
        default: command = 0; break;
    }
    
    IrReceiver.resume();
    return command;
  }
  return 0;
}
