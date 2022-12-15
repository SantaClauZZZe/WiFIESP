#include <SoftwareSerial.h> // EspSoftwareSerial

#include "WIFI.h"
#include "server.h"

#define PIN_TRIG 13 // ПИН D7
#define PIN_ECHO 12 // ПИН D6
#define ANALOG_PIN A0 // ПИН A0
#define LED_PIN 4 // ПИН D2

extern bool flag;

long duration, cm;
int s;

SoftwareSerial mySerial(14, 16); // RX — receive (принимаю), TX — transmit (передаю)

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
    digitalWrite(PIN_TRIG, LOW);   // Начало измерения расстояния
    delayMicroseconds(5);

    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(PIN_TRIG, LOW);

    duration = pulseIn(PIN_ECHO, HIGH);

    cm = (duration / 2) / 29.1;   // Результат измерений
    
    Serial.println("Distance -> " + String(cm));   // Отладочная инфа

    mySerial.println("1" + String(cm));   // Посылаем результат на ардуино (первая цифра означает вид измерения: 1 -> расстояние)

    delay(250);
  } 
  else  // Иначе измеренеие уровня освещённости
  {
    int value = analogRead(ANALOG_PIN);   // измеренеие уровня освещённости

    Serial.println("Brightness -> " + String(value));   // Отладочная инфа
    
    mySerial.println("0" + String(value));   // Посылаем результат на ардуино (первая цифра означает вид измерения: 0 -> уровень освещённости)
    
    delay(250);
  }

  while (Serial.available())   // Прием данных с порта (MQTT-python -> esp8266)
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
