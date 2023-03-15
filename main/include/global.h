#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <FastLED.h>


// IO utilities

#define LED_PIN 5
#define LED 2


// led matrix utilities

#define ROWS 8
#define COLS 19

extern uint8_t matrixArray[ROWS][COLS][3];
extern int ledMatrix[ROWS][COLS];
extern CRGB leds[ROWS*COLS];


// Bluetooth utilities

extern String state;
extern String color;
extern String speed;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define COLOR_CHARACTERISTIC_UUID "e963c47e-c96a-4aee-8859-922adb4ac93a"
#define MATRIX_CHARACTERISTIC_UUID "9f1b5ff8-b8ee-4e6c-b0be-668d85113b13"
#define SPEED_CHARACTERISTIC_UUID "89177ae3-2410-4eeb-b484-75861c2e108a"

extern BLEServer* pServer;
extern BLECharacteristic* pStatusCharacteristic;
extern BLECharacteristic* pColorCharacteristic;
extern BLECharacteristic* pMatrixCharacteristic;
extern BLECharacteristic* pSpeedCharacteristic;

extern bool deviceConnected;
extern bool oldDeviceConnected;

class characteristicCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic);
};

class serverCallbacks: public BLEServerCallbacks {
   void onConnect(BLEServer* pServer);
   void onDisconnect(BLEServer* pServer);
};
