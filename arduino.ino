#include <SoftwareSerial.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
}

void loop()
{
  if (Serial.available() > 0)
  {
    Serial.write(Serial.read() & 0xFF);
  }
}