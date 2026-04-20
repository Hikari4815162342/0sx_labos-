// #include "IRCommandReader.h"

// IRCommandReader::IRCommandReader(int pinIR){
//   pin = pinIR;
// }

// void IRCommandReader::init(){
//   IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);
// }

// int IRCommandReader::IRCommandUpdate(){
//   if (IrReceiver.decode()) {
//     unsigned long command = IrReceiver.decodedIRData.command;
//     if (IrReceiver.decodedIRData.flags && IRDATA_FLAGS_IS_REPEAT || command == 0) {
//       IrReceiver.resume();
//       return;
//     }

//     switch (command, HEX){
//     }

//     IrReceiver.resume();
//   }
// }