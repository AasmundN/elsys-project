#include <Arduino.h>

#include <global.h>

void task2() {
   if (state == "Hatt!")
      digitalWrite(LED, HIGH);
   else
      digitalWrite(LED, LOW);
}