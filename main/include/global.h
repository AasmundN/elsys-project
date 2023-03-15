#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <FastLED.h>

#define rows 8
#define cols 19

extern String state;
extern String color;

extern uint8_t matrixArray[rows][cols][3];

// IO utilities

#define LED_PIN 5
#define LED 2
#define NUM_LEDS 152
extern int ledMatrix[rows][cols];
extern CRGB leds[NUM_LEDS];


// Bluetooth utilities

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COLOR_CHARACTERISTIC_UUID "e963c47e-c96a-4aee-8859-922adb4ac93a"
#define MATRIX_CHARACTERISTIC_UUID "9f1b5ff8-b8ee-4e6c-b0be-668d85113b13"

extern BLEServer* pServer;
extern BLECharacteristic* pStatusCharacteristic;
extern BLECharacteristic* pColorCharacteristic;
extern BLECharacteristic* pMatrixCharacteristic;

extern bool deviceConnected;
extern bool oldDeviceConnected;

class characteristicCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic);
};

class serverCallbacks: public BLEServerCallbacks {
   void onConnect(BLEServer* pServer);
   void onDisconnect(BLEServer* pServer);
};
