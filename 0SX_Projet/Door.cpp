#include "Door.h"

Door::Door(Servo* door, HCSR04* hc, int PIN_SERVO){
  this->door = door;
  this-> hc = hc;
  this->pinServo = PIN_SERVO;
  this->angle = MIN_ANGLE;
  currentDoorState = CLOSED_DOOR;
  distance = 10;
  clients = 0;
}

void Door::init(){
  door->attach(this->pinServo);
  door->write(this->angle);
  door->detach();
}

DoorState Door::getState() const { return this->currentDoorState; }
int Door::getAngle() const { return this->angle; }
int Door::getClients() const { return this->clients; }
void Door::setState(DoorState state){ 
  this->currentDoorState = state;
  openFirstTime = true;
  waitFirstTime = true; 
}

void Door::readDistance(){
  if (currentTime - lastDistanceReading >= readingDelay){
    lastDistanceReading = currentTime;
    distance = hc->dist();
  }

  if (distance <= DIST_THRESHOLD && currentDoorState != WAIT && currentDoorState != EMERGENCY_DDOR){
    this->currentDoorState = OPEN;
  }
}

void Door::update(){
  currentTime = millis();

  if ((currentDoorState == OPEN || currentDoorState == CLOSE) && !door->attached()){
    door->attach(pinServo);
  }

  static unsigned long lastUpdate = 0;
  if (currentTime - lastUpdate >= interval){
    lastUpdate = currentTime;
    
    switch (currentDoorState){
      case OPEN:
        open();
        break;
      case WAIT:
        wait();
        break;
      case CLOSE:
        close();
        break;
      case EMERGENCY_DDOR:
        break;
      case CLOSED_DOOR:
        break;
    }
  }
}

void Door::open(){
  if (openFirstTime){
     clients++;
     openFirstTime = false;
  }
 
  if (angle < 170){
    ++angle;
  }else {
    openFirstTime = true;
    angle = 170;
    door->detach();
    this->currentDoorState = WAIT;
  }
  door->write(angle);
}

void Door::wait(){
  static unsigned long lastWaitTime;
  
  if (waitFirstTime){
    lastWaitTime = currentTime;
    waitFirstTime = false;
  }

  if (currentTime - lastWaitTime < WAIT_TIME){
    return;
  }

  waitFirstTime = true;
  this->currentDoorState = CLOSE;
}

void Door::close(){
  if (angle > 10){
    --angle;
  }else{
    angle = 10;
    door->detach();
    this->currentDoorState = CLOSED_DOOR;
  }
  door->write(angle);
}