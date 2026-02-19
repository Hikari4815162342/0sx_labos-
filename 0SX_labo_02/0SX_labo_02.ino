const int LED_PINS[] = { 7, 8, 9, 10 };
const int SIZE_LED = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const int POTENTIOMETER = A1;
int potentiometerValue = 0;
const int rangeOfValues = 20;
const int BUTTON = 2;

unsigned long currentTime = 0;
unsigned long lastTime = 0;
int currentState = 0;



void setup() {
  Serial.begin(9600);
  for (int i = 0; i < SIZE_LED; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
  pinMode(BUTTON, INPUT_PULLUP);
}

void switchLed(int mappedValue){
  int ledIndex = mappedValue / 5;
  for (int i = 0; i < SIZE_LED; i++){
    if (ledIndex == 4){
      digitalWrite(LED_PINS[SIZE_LED - 1],HIGH);
    }else if (i == ledIndex){
      digitalWrite(LED_PINS[i], HIGH);
    }else{
      digitalWrite(LED_PINS[i],LOW);
    }
  }
}

void loop() {
  currentTime = millis();
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

  int currentState = digitalRead(BUTTON);

  if (currentState != lastState) {
    lastTime = millis();
  }

  if (millis() - lastTime > delay && currentState != state) {
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
      Serial.print("!");
    }else{
      Serial.print(".");
    }
  }
  Serial.print("] ");
  Serial.print(percentage);
  Serial.println("%");
}


