#include <Arduino.h>

const int vol_pin = 3;

void setup() {

  Serial.begin(115200);

}

void loop() {
  int sensorValue = analogRead(vol_pin);
  Serial.println(sensorValue);
  delay(100);
}

