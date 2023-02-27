#include <Arduino.h>
#include <global.h>

String state = "";
String color = "";
String matrix = "";

BLEServer* pServer = NULL;
BLECharacteristic* pStatusCharacteristic = NULL;
BLECharacteristic* pColorCharacteristic = NULL;
BLECharacteristic* pMatrixCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {

   String * pHatParameter;

   const char * uuid = pCharacteristic->getUUID().toString().c_str();
   
   if (strcmp(uuid, STATUS_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &state;
   }
   if (strcmp(uuid, COLOR_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &color;
   }
   if (strcmp(uuid, MATRIX_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &matrix;
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
};

void serverCallbacks::onDisconnect(BLEServer* pServer) {
   deviceConnected = false;
}
