#include <Arduino.h>
#include <global.h>

String state = "";

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
   std::string value = pCharacteristic->getValue();

   if (value.length() > 0) {
      state = "";
      for (int i = 0; i < value.length(); i++)
         state += value[i];
      Serial.print(state);
   }
}

void serverCallbacks::onConnect(BLEServer* pServer) {
   deviceConnected = true;
};

void serverCallbacks::onDisconnect(BLEServer* pServer) {
   deviceConnected = false;
}
