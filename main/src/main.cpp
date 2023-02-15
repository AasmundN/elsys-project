#include <Arduino.h>
#include <test.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  test();
}