#include <Arduino.h>
#include <global.h>

String state = "sound";
String color = "";

BLEServer* pServer = NULL;
BLECharacteristic* pStatusCharacteristic = NULL;
BLECharacteristic* pColorCharacteristic = NULL;
BLECharacteristic* pMatrixCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {

   const char* uuid = pCharacteristic->getUUID().toString().c_str();

   // if the charactiristic is matrix
   if (strcmp(uuid, MATRIX_CHARACTERISTIC_UUID) == 0) {
      uint8_t* byteStream = pCharacteristic->getData();
      size_t length = pCharacteristic->getLength();

      for (int i = 0; i<length; i++) {
         Serial.print(i);
         Serial.print(": ");
         Serial.println(*(byteStream+i));
      }

      // update ledMatrix

      return;
   }

   String* pHatParameter;
   
   if (strcmp(uuid, STATUS_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &state;
   }

   if (strcmp(uuid, COLOR_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &color;
   }

   std::string value = pCharacteristic->getValue();

   if (value.length() > 0) {
      *pHatParameter = "";
      for (int i = 0; i < value.length(); i++)
         *pHatParameter += value[i];
      Serial.print("New parameter value: ");
      Serial.println(*pHatParameter);
   }
}

void serverCallbacks::onConnect(BLEServer* pServer) {
   deviceConnected = true;
}

void serverCallbacks::onDisconnect(BLEServer* pServer) {
   deviceConnected = false;
}
