#include "IRCommandReader.h"
#include "SerialReader.h"

int pinIR = 2;

IRCommandReader commandReader;
SerialReader serialReader;

void setup() {
  Serial.begin(9600);
  commandReader.init(pinIR);
}

void loop() {
  int command = commandReader.getCommand();
  if (command == 0){
    command = serialReader.getCommand();
  }

  delay(100);
}