#include "DCJoystick.h"

DCJoystick::DCJoystick(int xAxis, int yAxis, DCMotor* dcPtr){
  this->xAxis = xAxis;
  this->yAxis = yAxis;
  this->dcPtr = dcPtr;
}

int DCJoystick::readAndGetXValue(){
  return analogRead(this->xAxis);
}

int DCJoystick::readAndGetYValue(){
  return analogRead(this->yAxis);
}

void DCJoystick::update(){
  currentTime = millis();
  xValue = readAndGetXValue();
  yValue = readAndGetYValue();
  DCState state = OFF;
  static int speed = 60;
  static unsigned long lastTime = 0;
  
  if (yValue < 400){
    state = BACKWARD;
  }else if (yValue > 600){
    state = FORWARD;
  }else if (yValue >= 400 && yValue <= 600){
    state = OFF;
  }

  if (currentTime - lastTime >= speedInterval){
    lastTime = currentTime;
    if (xValue < 400){
      speed += 2;
    }else if (xValue > 600){
      speed -= 2;
    }
    Serial.println(speed);
    Serial.println(state);
  }
  
  if (speed >= dcPtr->getMaxSpeed()){
    speed = 255;
  }else if (speed <= dcPtr->getMinSpeed()){
    speed = 60;
  }

  if (currentTime - lastTime >= 1000){
    Serial.println(speed);
    Serial.println(state);
  }
  
  dcPtr->setState(state);
  dcPtr->setSpeed(speed);
}