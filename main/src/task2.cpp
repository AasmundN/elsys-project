#include <global.h>

void task2() {
   if (state == "sound")
      digitalWrite(LED, HIGH);
   else
      digitalWrite(LED, LOW);
}