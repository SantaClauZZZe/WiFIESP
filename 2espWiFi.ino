#include <SoftwareSerial.h>

#include "WIFI.h"
#include "server.h"

#define PIN_TRIG 13 // ПИН D7
#define PIN_ECHO 12 // ПИН D6
#define ANALOG_PIN A0 // ПИН A0
#define LED_PIN 4 // ПИН D2

extern bool flag;

long duration, cm;
int s;

SoftwareSerial mySerial(9, 10);

void setup()
{
  Serial.begin (9600); 

  while (!Serial) {}

  mySerial.begin (9600);
  while (!mySerial) {}

  WiFi_init(true);
  server_init();
  
  pinMode(PIN_TRIG, OUTPUT); // Определяем вывод
  pinMode(PIN_ECHO, INPUT); // Определяем ввод
  pinMode(ANALOG_PIN, INPUT); // Определяем ввод
  pinMode(LED_PIN, OUTPUT); // Определяем вывод
}

void loop()
{
  server.handleClient();

  if (flag)   // Измерение расстояния  с датчика
  {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);

    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(PIN_TRIG, LOW);

    duration = pulseIn(PIN_ECHO, HIGH);

    cm = (duration / 2) / 29.1;
    
    String ans = "1" + String(cm);

    Serial.println(ans);

    delay(250);
  } 
  else  // Иначе измеренеие уровня освещённости
  {
    int value = analogRead(ANALOG_PIN);

    Serial.println("0" + String(value));
    
    delay(250);
  }

  while (Serial.available())
  {
    s = Serial.read();
  }

  if (s == '1')
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else if (s == '0')
  {
    digitalWrite(LED_PIN, LOW);
  }
}