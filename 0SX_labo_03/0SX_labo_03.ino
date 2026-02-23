int thermistor = A0;
int thermoState = 0;
int led = 8;
int Vo;
float R1 = 10000; 
float logR2, R2, T, Tc;
float c1 = 1.129148e-03, c2 = 2.34125e-04, c3 = 8.76741e-08;

unsigned long serialTime = 0;
unsigned long serialLast = 0;
int serialDelay = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  controlCooling();
}

void controlCooling(){
  serialTime = millis();
  Vo = analogRead(thermistor);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15; 

  if (Tc > 35){
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }

  if (serialTime - serialLast >= serialDelay){
    serialLast = serialTime;
    Serial.print("Temperature: "); 
    Serial.println(Tc);
  }
}