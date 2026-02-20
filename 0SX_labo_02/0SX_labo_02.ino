const int LED_PINS[] = { 7, 8, 9, 10 };
const int SIZE_LED = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const int POTENTIOMETER = A1;
int potentiometerValue = 0;
const int rangeOfValues = 20;
const int BUTTON = 2;

unsigned long currentTime = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < SIZE_LED; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  potentiometerValue = analogRead(POTENTIOMETER);
  int mappedValue = map(potentiometerValue, 0, 1023, 0, rangeOfValues);
  switchLed(mappedValue);
  if (isClicked()){
    displayProgressBar(mappedValue, rangeOfValues);
  }
}

bool isClicked() {
  static int lastState = 1;
  static int state = 1;
  const int delay = 50;
  static unsigned long lastTime = 0;

  currentTime = millis();

  int currentState = digitalRead(BUTTON);

  if (currentState != lastState) {
    lastTime = currentTime;
  }

  if (currentTime - lastTime > delay && currentState != state) {
    state = currentState;
    if (state == 0) return true;
  }

  lastState = currentState;
  return false;
}

void displayProgressBar(int mappedValue, int rangeValues) {
  int percentage = (mappedValue * 100) / rangeValues;
  
  Serial.print("[");
  for (int i = 0; i < rangeValues; i++){
    if (i < mappedValue){
      Serial.print("!"); //Mon numéro d'étudiant est 2280684, donc mon avant dernier chiffre est pair donc ce doit être des points d'exclamation
    }else{
      Serial.print(".");
    }
  }
  Serial.print("] ");
  Serial.print(percentage);
  Serial.println("%");
}

void switchLed(int mappedValue){
  int ledIndex = mappedValue / 5;

  if (ledIndex >= SIZE_LED){
    ledIndex = SIZE_LED - 1;
  }

  for (int i = 0; i < SIZE_LED; i++){ //Mon numéro d'étudiant est 2280684, donc mon dernier chiffre est pair donc j'allume une
    if (i == ledIndex){               //led selon la position du potentiomètre
      digitalWrite(LED_PINS[i], HIGH);
    }else{
      digitalWrite(LED_PINS[i],LOW);
    }
  }
}



