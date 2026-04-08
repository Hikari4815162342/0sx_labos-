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