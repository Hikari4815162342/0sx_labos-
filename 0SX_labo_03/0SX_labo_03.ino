String etd = "2280684"; //Mon numéro d'étudiant est pair
int thermistor = A0;
int led = 8;
int Vo;
float R1 = 10000; 
float logR2, R2, T, Tc;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;

unsigned long currentTime = 0;
unsigned long serialLast = 0;
int serialDelay = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
}

void loop() {
  currentTime = millis();
  controlCooling();
  printSerial(currentTime);
}

bool controlCooling(){ //Mon numéro d'étudiant est pair donc je dois vérifier la temnpérature
  Vo = analogRead(thermistor);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15; 

  if (Tc > 25){
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
