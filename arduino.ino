#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX — receive (принимаю), TX — transmit (передаю)

void setup()
{
    pinMode(2, INPUT);

    Serial.begin(9600);
    while (!Serial) {}

    mySerial.begin (9600);
    while (!mySerial) {}
}

void loop()
{
    if (mySerial.available() > 0) // Если есть данные с SoftwareSerial
    {
        Serial.write(mySerial.read() & 0xFF); // Перенаправление данных (esp8266 -> MQTT-python)
    }
}