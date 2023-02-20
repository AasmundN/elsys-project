#include <Arduino.h>

#include <global.h>

void task2() {
   write_led = !write_led;
   delay(500);
}