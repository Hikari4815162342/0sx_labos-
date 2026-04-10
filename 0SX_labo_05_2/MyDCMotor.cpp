#include "MyDCMotor.h"

DCMotor::DCMotor(int input1, int input2){
  this->input1 = input1;
  this->input2 = input2;
  pinMode(this->input1, OUTPUT);
  pinMode(this->input2, OUTPUT);
  this->speed = 60;
  this->maxSpeed = 255;
  this->minSpeed = 60;
  this->currentState = OFF;
}

DCState DCMotor::getState() const { return this->currentState; }
int DCMotor::getSpeed() const { return this->speed; }
void DCMotor::setSpeed(int speed) { 
  if (speed >= minSpeed && speed <= maxSpeed){
    this->speed = speed;
  }
}
void DCMotor::setState(DCState currentState) { this->currentState = currentState; };
bool DCMotor::isActive(){
  if (this->currentState == FORWARD || this->currentState == BACKWARD){
    return true;
  }else{
    return false;
  }
}

void DCMotor::updateDCMotor(){
  switch (this->currentState){
    case FORWARD:
      forward();
      break;
    case BACKWARD:
      backward();
      break;
    case OFF:
      off();
      break;
    case EMERGENCY:
      off();
      break;
  }
}

void DCMotor::forward(){
  analogWrite(this->input1, this->speed);
  analogWrite(this->input2, LOW); 
}

void DCMotor::backward(){
  analogWrite(this->input1, LOW);
  analogWrite(this->input2, this->speed);  
}

void DCMotor::off(){
  analogWrite(this->input1, LOW); 
  analogWrite(this->input2, LOW); 
}