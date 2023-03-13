#include <global.h>

#include <setup.h>
#include <task1.h>
#include <task2.h>

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//Task1code: blinks an LED every 1000 ms
void Task1code( void* pvParameters ) {
   Serial.print("Task1 running on core ");
   Serial.println(xPortGetCoreID());

   for(;;) {
      // task 1 code
      task1();
      // do not remove delay: 
      // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time
      vTaskDelay(1);
   }
}

//Task2code: blinks an LED every 700 ms
void Task2code( void* pvParameters ) {
   Serial.print("Task2 running on core ");
   Serial.println(xPortGetCoreID());

   for(;;) {
      // task 2 code
      task2();
      // do not remove delay: 
      // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time
      vTaskDelay(1);
   }
}

void setupTasks(TaskHandle_t Task1, TaskHandle_t Task2) {

   //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
   xTaskCreatePinnedToCore(
                  Task1code,   /* Task function. */
                  "Task1",     /* name of task. */
                  10000,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  1,           /* priority of the task */
                  &Task1,      /* Task handle to keep track of created task */
                  0);          /* pin task to core 0 */                  
   delay(500); 

   //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
   xTaskCreatePinnedToCore(
                  Task2code,   /* Task function. */
                  "Task2",     /* name of task. */
                  10000,       /* Stack size of task */
                  NULL,        /* parameter of the task */
                  1,           /* priority of the task */
                  &Task2,      /* Task handle to keep track of created task */
                  1);          /* pin task to core 1 */
   delay(500); 
}

void setupBluetooth() {
   BLEDevice::init("ESP32");

   // start server
   pServer = BLEDevice::createServer();
   pServer->setCallbacks(new serverCallbacks());

   // start service
   BLEService* pService = pServer->createService(SERVICE_UUID);

   // create characteristics
   pStatusCharacteristic = pService->createCharacteristic(
                                          STATUS_CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   pColorCharacteristic = pService->createCharacteristic(
                                          COLOR_CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   pMatrixCharacteristic = pService->createCharacteristic(
                                          MATRIX_CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   // set callbacks
   characteristicCallbacks *callbacks = new characteristicCallbacks();
   pStatusCharacteristic->setCallbacks(callbacks);
   pColorCharacteristic->setCallbacks(callbacks);
   pMatrixCharacteristic->setCallbacks(callbacks);

   // set inital characteristic value
   pStatusCharacteristic->setValue("");
   pColorCharacteristic->setValue("");
   pMatrixCharacteristic->setValue("");
   pService->start();
   
   // Start advertising
   BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
   pAdvertising->addServiceUUID(SERVICE_UUID);
   pAdvertising->setScanResponse(false);
   pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
   BLEDevice::startAdvertising();

   Serial.println("Bluetooth setup finished");
}