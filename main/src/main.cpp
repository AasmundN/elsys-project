// libraries
#include <Arduino.h>

// custom files
#include <setup.h>
#include <global.h>

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

// void setup
void setup() {
   // setup Serial
   Serial.begin(115200); 

   // setup functions
   setupBluetooth();
   setupMatrix();
   setupTasks(Task1, Task2, Task3);

   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

   Serial.println("------------------------------------");
   Serial.println("-            Setup done            -");
   Serial.println("------------------------------------");
}

void loop() {
   
}
