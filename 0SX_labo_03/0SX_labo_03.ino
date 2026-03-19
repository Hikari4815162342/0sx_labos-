#include <LCD_I2C.h> 

String etd = "2280684"; //Mon numéro d'étudiant est pair
const int SIZE_ETD = 7;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2; 
int thermistor = A0;
int led = 8;
int xAxis = A1;
int yAxis = A2;
int btn = 2;
int Vo;
float R1 = 10000; 
float logR2, R2, T, Tc;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;
const short TEMP_THRESHOLD = 35;

unsigned long currentTime = 0;
unsigned long serialLast = 0;
int interval = 1000;

LCD_I2C lcd(0x27, LCD_COLS, LCD_ROWS); 

uint8_t persoChar[8] = {0b11100, 0b10100, 0b11100, 0b10101, 0b11101, 0b00111, 0b00001, 0b00001};

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  lcdSetup();
}

void loop() {
  currentTime = millis();
  controlCooling();
  joystickTask();
}
bool isClicked() {
  static int lastState = 1;
  static int state = 1;
  const int delay = 50;
  static unsigned long lastTime = 0;

  int currentState = digitalRead(btn);

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

void lcdSetup(){
  lcd.begin();
  lcd.backlight();
  lcd.createChar(0, persoChar);
  
  lcd.print("Marchand");
  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.setCursor((LCD_COLS - SIZE_ETD), 1);
  lcd.print("*****84");

  delay(1000);
  lcd.clear();
}

void joystickTask(){
  static unsigned long lastAltitudeTime = 0;
  
  int valueX = analogRead(xAxis);
  int valueY = analogRead(yAxis);
  
  static int altitude = 0;
  int direction = map(valueX,0,1022,-90,90);

  if (direction > -5 && direction < 5) {
    direction = 0;  
  }

  if (currentTime - lastAltitudeTime >= interval){
    if (valueY > 800 && altitude > 0){
      lastAltitudeTime = currentTime;
      altitude--;
    }else if (valueY < 200 && altitude < 200){
      lastAltitudeTime = currentTime;
      altitude++;
    }
  }
  displayLcdInfos(direction,valueY,altitude);
  printSerial(valueX, valueY);
}

void displayLcdInfos(int direction, int yValue, int altitude){
  static int lcdState = 0;
  bool cooling = controlCooling();

  switch (lcdState){
    case 0:
      if (!isClicked()){
        lcd.setCursor(0,0);
        lcd.print("ALT: ");
        lcd.print(altitude);
        lcd.print("m");
        if (yValue < 800 && yValue > 200){
          lcd.print("           ");
        }else if (yValue < 500){
          lcd.print("(UP)     ");
        }else{
          lcd.print("(DOWN)   ");
        }
        lcd.setCursor(0,1);
        lcd.print("DIR: ");
        lcd.print(direction);
        if (direction < 0){
          lcd.print("(G)   ");
        }else if (direction == 0){
          lcd.print("     ");
        }else{
          lcd.print("(D)   ");
        }
      }else{
        lcdState = 1;
        lcd.clear();
      }
      break;
    case 1:
      if (!isClicked()){
        lcd.setCursor(0,0);
        lcd.print(Tc);
        lcd.print(" C");
        lcd.setCursor(0,1);
        lcd.print("COOL: ");
        if (cooling){
          lcd.print("ON   ");
        }else{
          lcd.print("OFF  ");
        }
      }else{
        lcdState = 0;
        lcd.clear();
      }
      break;
  }
}

bool controlCooling(){ //Mon numéro d'étudiant est pair donc je dois vérifier la temnpérature
  Vo = analogRead(thermistor);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15; 

  if (Tc > TEMP_THRESHOLD){
    digitalWrite(led, HIGH);
    return true;
  }else{
    digitalWrite(led, LOW);
  }
  return false;
}

void printSerial(int xValue, int yValue){
  bool cooling = controlCooling();
  if (currentTime - serialLast >= interval){
    serialLast = currentTime;
    Serial.print("etd:"); 
    Serial.print(etd);
    Serial.print(",x:");
    Serial.print(xValue);
    Serial.print(",y:");
    Serial.print(yValue);
    Serial.print(",sys:");
    Serial.println(cooling);
  }
}
