#include <Arduino.h>
#include <global.h>

String state = "motion";
String fill = "6";
String speed = "0";

ADCSampler *adcSampler = NULL;
int16_t *samples = NULL;   
I2SSampler *sampler = NULL; 

uint8_t ledMatrix[ROWS][COLS][3];
int ledMatrixIndices[ROWS][COLS];
CRGB leds[ROWS*COLS];

bool updatingMatrix = false;

void updateMatrix() {
   //Kode som oppdaterer led matrise
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         // get new LED color values
         uint8_t red = ledMatrix[row][col][0];
         uint8_t green = ledMatrix[row][col][1];
         uint8_t blue = ledMatrix[row][col][2];

         // update LED color values
         leds[ledMatrixIndices[row][col]] = CRGB(red, green, blue);
      }
   }

   FastLED.show();
}

BLEServer* pServer = NULL;
BLECharacteristic* pStatusCharacteristic = NULL;
BLECharacteristic* pFillCharacteristic = NULL;
BLECharacteristic* pMatrixCharacteristic = NULL;
BLECharacteristic* pSpeedCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;

void characteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {

   const char* uuid = pCharacteristic->getUUID().toString().c_str();
   String* pHatParameter = NULL;

   // if the charactiristic is matrix
   if (strcmp(uuid, MATRIX_CHARACTERISTIC_UUID) == 0) { 

      updatingMatrix = true;

      uint8_t* byteStream = pCharacteristic->getData();
      size_t length = pCharacteristic->getLength();

      // update LED matrix
      int arrIndex = 0;
      for (int row = 0; row<ROWS; row++) {
         for (int col = 0; col<COLS; col++) {
            for (int k = 0; k<3; k++){
               Serial.print(arrIndex);
               Serial.print(": ");
               Serial.println(byteStream[arrIndex]);
               ledMatrix[row][col][k] = byteStream[arrIndex];
               arrIndex++;
            }
         }
      }

      updateMatrix();

      updatingMatrix = false;
      return;
   }

   // check what characteristic is being written

   if (strcmp(uuid, STATUS_CHARACTERISTIC_UUID) == 0) pHatParameter = &state;

   if (strcmp(uuid, FILL_CHARACTERISTIC_UUID) == 0) pHatParameter = &fill;
   
   if (strcmp(uuid, SPEED_CHARACTERISTIC_UUID) == 0) pHatParameter = &speed;


   // save written value
   
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
   Serial.println("Device connected");
}

void serverCallbacks::onDisconnect(BLEServer* pServer) {
   delay(500); // give the bluetooth stack the chance to get things ready
   pServer->startAdvertising(); // restart advertising
   Serial.println("Device disconnected, restarting advertising");
}