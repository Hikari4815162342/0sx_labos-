#include <Arduino.h>
#include <IRremote.hpp>
#include "IRCommandReader.h"

IRCommandReader::IRCommandReader(){};

void IRCommandReader::init(int pin){
  IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);
}

bool IRCommandReader::getEvent(int& command){
  if (!IrReceiver.decode()){
    return false;
  }
  
  if (IrReceiver.decodedIRData.flags && IRDATA_FLAGS_IS_REPEAT) {
    IrReceiver.resume();
    return false;
  }

  int cmd = IrReceiver.decodedIRData.command;

  IrReceiver.resume();

  switch (cmd){
    case 12: command = 1; break;
    case 24: command = 2; break;
    case 94: command = 3; break;
    case 8: command = 4; break;
    default: return false;
  }
    
    return true;
}