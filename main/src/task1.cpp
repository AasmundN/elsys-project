#include <global.h>

void task1() {
   // disconnecting
   if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("Device disconnected, restarting advertising");
      oldDeviceConnected = deviceConnected;
   }
   // connecting
   if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      Serial.println("Device connected");
      oldDeviceConnected = deviceConnected;
   }
}