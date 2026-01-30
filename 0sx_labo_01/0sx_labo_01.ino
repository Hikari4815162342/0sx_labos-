enum StateSwitch {
  ON_OFF,
  FADE,
  BLINK
};
int led = LED_BUILTIN;
int brightness = 0;
int fadeAmount = 5;
String etd = "2280684";
StateSwitch currentState = BLINK;
const short NB_BLINK = 4;

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

void displayState(StateSwitch currentState){
  String state;
  if (currentState == BLINK){
    state = "Clignotement";
  }else if (currentState == FADE){
    state = "Varie";
  }else{
    state = "Allume";
  }
  Serial.print("Message : << Etat : ");
  Serial.print(state);
  Serial.print(" - ");
  Serial.print(etd);
  Serial.println(" >>");
  delay(500);
}

void blink(){
  //Clignote 4 fois selon mon numero d'admission
  for (int i = 0; i < NB_BLINK; i++){
    digitalWrite(led,HIGH);
    delay(350);
    digitalWrite(led,LOW);
    delay(350);
  }
}

void fade(){


}

void loop() {
  switch(currentState){
    //Commence a blink
    case BLINK:
      Serial.println("Début de blink");
      delay(500);
      blink();
      displayState(currentState);
      currentState = FADE;
      break;
    case FADE:
      Serial.println("Début de fade");
      delay(500);
      fade();
      displayState(currentState);
      currentState = ON_OFF;
      break;
    // case ON_OFF:
    //   Serial.println("Début de fade");
    //   delay(500);
    //   on_off();
    //   displayState(currentState);
    //   currentState = BLINK;
    //   break;
  }

}
