#include <HCSR04.h>
#include <Servo.h>
#include "Door.h"
#include "LcdManager.h"
#include <OneButton.h>
#include <LCD_I2C.h>
#include "MyDCMotor.h"
#include "DCJoystick.h"
#include "DCLeds.h"
#include "LED.h"
#include "LEDMatrix.h"
#include "SerialReader.h" 
#include "IRCommandReader.h"
#include <U8g2lib.h>
#include <WiFiEspAT.h>
#include <PubSubClient.h>

#define HAS_SECRETS 1
#define DEVICE_NAME "Raphael"

#if HAS_SECRETS
#include "arduino_secrets.h"

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

#endif

#define AT_BAUD_RATE 115200

#define MQTT_PORT 1883
#define MQTT_USER "etdshawi"
#define MQTT_PASS "shawi123"

const char* mqttServer = "216.128.180.194";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

//Variables et objets labo4
#define TRIGGER_PIN 7
#define ECHO_PIN 8
const int PIN_OPEN_BTN = 11;
const int PIN_EMERGENCY_BTN = 12;
const int PIN_SERVO = 6;

Servo servo;
OneButton openBtn = OneButton(PIN_OPEN_BTN, true, true);
OneButton emergencyBtn = OneButton(PIN_EMERGENCY_BTN, true, true);
LCD_I2C lcd(0x27, 16, 2);
HCSR04 hc(TRIGGER_PIN, ECHO_PIN);
const int DIST_THRESHOLD = 5;

Door door(&servo, &hc, PIN_SERVO);

//Variables et objets labo5
int input1 = 4;
int input2 = 3;
int xAxis = A0;
int yAxis = A1;

int emergencyLedPin = 10;
int activeLedPin = 9;

DCMotor myDC(input1, input2);
DCJoystick DCJoystick(xAxis, yAxis, &myDC);
LED emergencyLed(emergencyLedPin);
LED activeLed(activeLedPin);
DCLeds DCLeds(&activeLed, &emergencyLed, &myDC);

//Variables et objet labo6
#define CLK_PIN  30
#define DIN_PIN  34
#define CS_PIN   32

U8G2_MAX7219_8X8_F_4W_SW_SPI u8g2(
  U8G2_R0,
  CLK_PIN,
  DIN_PIN,
  CS_PIN,
  U8X8_PIN_NONE,
  U8X8_PIN_NONE
);

unsigned long currentTime = 0;
unsigned long lastTime = 0;
int interval = 500;

int pinIR = 2;
int command = 0;

LEDMatrix ledMatrix(&u8g2);
IRCommandReader commandReader;
SerialReader serialReader;

LcdManager screen(&lcd, &door, &myDC, &ledMatrix);

void setup() {
  Serial.begin(AT_BAUD_RATE);
  lcd.begin();
  lcd.backlight();

  door.init();

  openBtn.attachClick(onClickOpenDoor);
  emergencyBtn.attachClick(onClickEmergency);

  DCLeds.begin();

  commandReader.init(pinIR);
  ledMatrix.init();

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  wifiInit();

  client.setServer(mqttServer, MQTT_PORT);
  client.setCallback(mqttEvent);

  if(!client.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS)) {
    Serial.println("Incapable de se connecter sur le serveur MQTT");
    Serial.print("client.state : ");
    Serial.println(client.state());
  } else{
    Serial.println("Connecté sur le serveur MQTT");
  }

  client.subscribe("magasin/8/set", 0);
  delay(1000);
}


 void loop() { 
  currentTime = millis();
  openBtn.tick();
  emergencyBtn.tick();
  DoorState doorState = door.getState();
  DCState dcState = myDC.getState();
  LEDMatrixModes mode = ledMatrix.getMode();
  
  if (doorState != EMERGENCY_DDOR && dcState != EMERGENCY_DC && mode != EMERGENCY_MATRIX){
    if (commandReader.getEvent(command)){
      ledMatrix.treatCommand(command);
    }else if (serialReader.commandReceived()){
      command = serialReader.getCommand();
      ledMatrix.treatCommand(command);
    }

    door.readDistance();
    DCJoystick.update();
    myDC.updateDCMotor();
  }

  if (currentTime - lastTime >= interval){
    lastTime = currentTime;
    u8g2.clearBuffer();
    ledMatrix.update();
    u8g2.sendBuffer();
  }

  door.update();
  DCLeds.update();
  screen.update();
  client.loop();
  publishInfos();
}


void onClickOpenDoor(){
  DoorState state = door.getState();
  if (state == CLOSED_DOOR || state == CLOSE){
    door.setState(OPEN);
  }
}

void onClickEmergency(){
  DoorState doorState = door.getState();
  DCState dcState = myDC.getState();
  LEDMatrixModes mode = ledMatrix.getMode();

  if (doorState == EMERGENCY_DDOR && dcState == EMERGENCY_DC && mode == EMERGENCY_MATRIX){
    command = 0;
    ledMatrix.setMode(SHUT_OFF);
    door.setState(CLOSE);
    myDC.setState(OFF);
    screen.setScreen(SCREEN_1);
  }else{
    ledMatrix.setMode(EMERGENCY_MATRIX);
    door.setState(EMERGENCY_DDOR);
    myDC.setState(EMERGENCY_DC);
    screen.setScreen(EMERGENCY_SCREEN);
  }
}

void wifiInit() {
  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println();
    Serial.println("La communication avec le module WiFi a échoué!");
    while (true) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(50);
    }
  }

  Serial.println("En attente de connexion au WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println();

  IPAddress ip = WiFi.localIP();
  Serial.println();
  Serial.println("Connecté au réseau WiFi.");
  Serial.print("Adresse : ");
  Serial.println(ip);

  printWifiStatus();
}

void printWifiStatus() {

  char ssid[33];
  WiFi.SSID(ssid);
  Serial.print("SSID: ");
  Serial.println(ssid);

  uint8_t bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  IPAddress ip = WiFi.localIP();
  Serial.print("Adresse IP: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("force du signal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}



void mqttEvent(char* topic, byte* payload, unsigned int length){
  Serial.print("Message recu [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char payloadStr[100];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  if (strcmp(topic, "magasin/8/set") == 0){
    if (strstr(payloadStr, "rabais")){
      ledMatrix.setMode(DISCOUNT);
    }else if (strstr(payloadStr, "urgence")){
      ledMatrix.setMode(EMERGENCY_MATRIX);
    }else if (strstr(payloadStr, "erreur")){
      ledMatrix.setMode(ERROR);
    }else if (strstr(payloadStr, "ok")){
      ledMatrix.setMode(NORMAL);
    }
  }
}

void publishInfos(){
  static unsigned long lastPublishTime = 0;
  const unsigned int rate = 10000;

  static char message[200] = "";
  static char sDirection[10] = "";
  static char sMatrixState[10] = "";
  static char sDoorState[10] = "";

  DCState dcState = myDC.getState();
  int speed = myDC.getSpeed();
  LEDMatrixModes mode = ledMatrix.getMode();
  DoorState doorState = door.getState();
  int nbClients = door.getClients();
  
  if (currentTime - lastPublishTime < rate) return;

  lastPublishTime = currentTime;

  switch (dcState){
    case FORWARD:
      strcpy(sDirection, "droite");
      break;
    case BACKWARD:
      strcpy(sDirection, "gauche");
      break;
    case OFF:
      strcpy(sDirection, "stop");
      break;
  }

  switch (doorState){
    case OPEN:
      strcpy(sDoorState, "move");
      break;
    case WAIT:
      strcpy(sDoorState, "ouvert");
      break;
    case CLOSED_DOOR:
      strcpy(sDoorState, "ferme");
      break;
  }

  switch(mode){
    case DISCOUNT:
      strcpy(sMatrixState, "rabais");
      break;
    case NORMAL: 
      strcpy(sMatrixState, "ok");
      break;
    case SHUT_OFF:
      strcpy(sMatrixState, "ok");
      break;
    case ERROR: 
      strcpy(sMatrixState, "erreur");
      break;
    case EMERGENCY_MATRIX: 
      strcpy(sMatrixState, "urgence");
      break;
  }

  sprintf(message, 
      "{\"direction\":\"%s\","
      "\"vitesse\":%d,"
      "\"porte\":\"%s\","
      "\"statut\":\"%s\","
      "\"clients\":%d}",
      sDirection, speed, sDoorState, sMatrixState, nbClients);

  Serial.print("Envoie : ");
  Serial.println(message);

  if (!client.publish("magasin/8/state", message)){
    reconnect();
    Serial.println("Incapable d'envoyer le message!");
  }else{
    Serial.println("Message envoyé");
  }
}

bool reconnect() {
  bool result = client.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS);
  if(!result) {
    Serial.println("Incapable de se connecter sur le serveur MQTT");
  }
  return result;
}



