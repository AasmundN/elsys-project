#include <Arduino.h>

#include <global.h>

void task1() {
   if (write_led)
      digitalWrite(LED, HIGH);  
   else
      digitalWrite(LED, LOW);
}