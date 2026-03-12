#include <LCD_I2C.h> 

String etd = "2280684"; //Mon numéro d'étudiant est pair
const int SIZE_ETD = 7;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2; 
int thermistor = A0;
int led = 8;
int Vo;
float R1 = 10000; 
float logR2, R2, T, Tc;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;

unsigned long currentTime = 0;
unsigned long serialLast = 0;
int serialDelay = 1000;

LCD_I2C lcd(0x27, LCD_COLS, LCD_ROWS); 

uint8_t persoChar[8] = {0b11100, 0b10100, 0b11100, 0b10101, 0b11101, 0b00111, 0b00001, 0b00001};

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  lcdSetup();
}

void loop() {
  currentTime = millis();
  controlCooling();
  printSerial(currentTime);
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

  delay(3000);
  lcd.clear();
}

bool controlCooling(){ //Mon numéro d'étudiant est pair donc je dois vérifier la temnpérature
  Vo = analogRead(thermistor);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15; 

  if (Tc > 35){
    digitalWrite(led, HIGH);
    return true;
  }else{
    digitalWrite(led, LOW);
  }
  return false;
}

void printSerial(unsigned long serialTime){
  bool cooling = controlCooling();
  if (currentTime - serialLast >= serialDelay){
    serialLast = currentTime;
    Serial.print("etd:"); 
    Serial.print(etd);
    Serial.print(",x: ");
    Serial.print(",y: ");
    Serial.print(",sys:");
    Serial.println(cooling);
  }

}
