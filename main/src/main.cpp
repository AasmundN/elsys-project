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
   
   Serial.println();
   Serial.println("------------------------------------");
   Serial.println("-          Starting setup          -");
   Serial.println("------------------------------------");
   Serial.println();

   // setup functions
   setupMatrix();
   setupBluetooth();
   setupTasks(Task1, Task2);

   // setup built in LED
   pinMode(LED, OUTPUT);
   digitalWrite(LED, LOW);

   Serial.println();
   Serial.println("------------------------------------");
   Serial.println("-            Setup done            -");
   Serial.println("------------------------------------");
   Serial.println();
}

void loop() {
   
}
