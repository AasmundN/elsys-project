#include <Arduino.h>

#include <global.h>

void task1() {
   // put your main code here, to run repeatedly:
   // disconnecting
   if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
   }
   // connecting
   if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
   }
}