enum StateSwitch {
  ON_OFF,
  FADE,
  BLINK
};
int led = LED_BUILTIN;
//Numero d'admission
String etd = "2280684";
StateSwitch currentState = BLINK;
const short NB_BLINK = 4;

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
}

//Fonction pour afficher l'état en cours
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
  //Varie de 0 jusqu'à 255 (0 à 100%, selon mon numero d'admission) pendant 2048ms
  for (int brightness = 0; brightness <= 255; brightness++){
    analogWrite(led,brightness);
    delay(8);
  }
}

void on_off(){
  //S'éteint durant 300ms
  digitalWrite(led,LOW);
  delay(300);
  //S'allume pendant 2sec
  digitalWrite(led,HIGH);
  delay(2000);
  //S'éteint pendant 1sec
  digitalWrite(led,LOW);
  delay(1000);
}

void loop() {
  switch(currentState){
    case BLINK:
      blink();
      displayState(currentState);
      currentState = FADE;
      break;
    case FADE:
      fade();
      displayState(currentState);
      currentState = ON_OFF;
      break;
    case ON_OFF:
      on_off();
      displayState(currentState);
      currentState = BLINK;
      break;
  }
}
