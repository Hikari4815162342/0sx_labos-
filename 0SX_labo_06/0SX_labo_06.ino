//#include "IRCommandReader.h"
#include <IRremote.hpp>

int pinIR = 2;
//IRCommandReader irCommand(pinIR);

void setup() {
  Serial.begin(9600);
  //irCommand.init();
  IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long command = IrReceiver.decodedIRData.command;
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

    Serial.println(command);
    
    IrReceiver.resume();
  }
}