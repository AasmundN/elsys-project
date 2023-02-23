#include <global.h>

bool write_led = false;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
   std::string value = pCharacteristic->getValue();

   if (value.length() > 0) {
      Serial.println("*********");
      Serial.print("New value: ");
      for (int i = 0; i < value.length(); i++) {
         Serial.print(value[i]);
      }

      Serial.println();
      Serial.println("*********");
   }
}

void serverCallbacks::onConnect(BLEServer* pServer) {
   deviceConnected = true;
};

void serverCallbacks::onDisconnect(BLEServer* pServer) {
   deviceConnected = false;
}
