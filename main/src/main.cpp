// libraries
#include <Arduino.h>

// custom files
#include <setup.h>
#include <global.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

// void setup
void setup() {
   // setup Serial
   Serial.begin(115200); 

   // setup functions
   setupTasks(Task1, Task2);
   pinMode(LED, OUTPUT);

   Serial.println("------------------------------------");
   Serial.println("-            Setup done            -");
   Serial.println("------------------------------------");
}

void loop() {
   
}
