#include "DCLeds.h"

DCLeds::DCLeds(LED* activeLed, LED* emergencyLed, DCMotor* dc){
  this->activeLed = activeLed;
  this->emergencyLed = emergencyLed;
  this->dc = dc;
}

void DCLeds::begin(){
  activeLed->init();
  emergencyLed->init();
}

void DCLeds::update(){
  DCState state = dc->getState();
  if (state != EMERGENCY){
    emergencyLed->off();
    if (state == FORWARD || state == BACKWARD){
      activeLed->on();
    }else{
      activeLed->off();
    }  
  }else if (state == EMERGENCY){
    activeLed->off();
    emergencyLed->on();
  }
}
