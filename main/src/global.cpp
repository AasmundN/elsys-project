#include <Arduino.h>
#include <global.h>

String state = "sound";
String color = "";
String speed = "";

int matrixArray[rows][cols][3];

BLEServer* pServer = NULL;
BLECharacteristic* pStatusCharacteristic = NULL;
BLECharacteristic* pColorCharacteristic = NULL;
BLECharacteristic* pMatrixCharacteristic = NULL;
BLECharacteristic* pSpeedCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {

   const char* uuid = pCharacteristic->getUUID().toString().c_str();
   String* pHatParameter = NULL;

   // if the charactiristic is matrix
   if (strcmp(uuid, MATRIX_CHARACTERISTIC_UUID) == 0){ 
      uint8_t* byteStream = pCharacteristic->getData();
      size_t length = pCharacteristic->getLength();

      int arrIndex = 0;
      for (int i = 0; i<rows; i++) {
         for (int j = 0; j<cols; j++){
            for (int k = 0; k<3; k++){
               Serial.print(arrIndex);
               Serial.print(": ");
               Serial.println(byteStream[arrIndex]);
               matrixArray[i][j][k] = byteStream[arrIndex];
               arrIndex++;
            }
         }
      }
      return;
   }

   // if the characteristic is status or color
   if (strcmp(uuid, STATUS_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &state;
   }

   if (strcmp(uuid, COLOR_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &color;
   }
   
   if (strcmp(uuid, SPEED_CHARACTERISTIC_UUID) == 0) {
      pHatParameter = &speed;
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