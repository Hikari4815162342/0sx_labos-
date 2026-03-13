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
int serialDelay = 2000;

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
  readJoystickValue(currentTime);
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

void readJoystickValue(unsigned long currentTime){
  static unsigned long lastTime = 0;
  int valueX = analogRead(xAxis);
  delay(100);
  int valueY = analogRead(yAxis);
  static int altitude = 0;
  static int direction = 0;
  
  switch (valueY){
    case 1023:
      if (currentTime - lastTime >= 1000){
        lastTime = currentTime;
        if (altitude < 200){
          altitude++;
        }
      }
      break;
    case 0:
      if (currentTime - lastTime >= 1000){
        lastTime = currentTime;
        if (altitude > 0){
          altitude--;
        }
      }
      break;
  }s

  switch (valueX){
     case 1023:
      if (currentTime - lastTime >= 1000){
        lastTime = currentTime;
        if (direction < 90){
          direction++;
        }
      }
      break;
    case 0:
      if (currentTime - lastTime >= 1000){
        lastTime = currentTime;
        if (direction > -90){
          direction--;
        }
      }
      break;
  }
  printSerial(currentTime, valueX, valueY, altitude);
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

void printSerial(unsigned long serialTime, int xValue, int yValue, int altitude){
  bool cooling = controlCooling();
  if (serialTime - serialLast >= serialDelay){
    serialLast = serialTime;
    Serial.print("etd:"); 
    Serial.print(etd);
    Serial.print(",x: ");
    Serial.print(xValue);
    Serial.print(",y: ");
    Serial.print(yValue);
    Serial.print(",sys:");
    Serial.print(cooling);
    Serial.print(", alt:");
    Serial.println(altitude);
  }


}
