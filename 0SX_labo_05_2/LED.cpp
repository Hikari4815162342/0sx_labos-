#include "LED.h"

LED::LED(int pin){
  this->pin = pin;
}

void LED::init(){
  pinMode(this->pin, OUTPUT);
}

void LED::on(){
  digitalWrite(this->pin, HIGH);
}

void LED::off(){
  digitalWrite(this->pin, LOW);
}
