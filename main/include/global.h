#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <FastLED.h>
#include "ADCSampler.h"
#include <Wire.h>


// IO utilities

#define LED_PIN 5
#define LED 2
#define AUDIO_IN_PIN 35 
const int MPU_addr=0x68;

// I2S utilities

#define I2S_SAMPLE_RATE 20000
#define SAMPLE_SIZE  256

extern ADCSampler *adcSampler;
extern int16_t *samples;   
extern I2SSampler *sampler;   

// led matrix utilities

#define ROWS 8
#define COLS 19

extern uint8_t ledMatrix[ROWS][COLS][3];
extern int ledMatrixIndices[ROWS][COLS];
extern CRGB leds[ROWS*COLS];

void updateMatrix();

// Bluetooth utilities

extern String state;
extern String fill;
extern String speed;

extern bool updatingMatrix;

#define SERVICE_UUID                 "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define STATUS_CHARACTERISTIC_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define FILL_CHARACTERISTIC_UUID    "e963c47e-c96a-4aee-8859-922adb4ac93a"
#define MATRIX_CHARACTERISTIC_UUID   "9f1b5ff8-b8ee-4e6c-b0be-668d85113b13"
#define SPEED_CHARACTERISTIC_UUID    "89177ae3-2410-4eeb-b484-75861c2e108a"

extern BLEServer* pServer;
extern BLECharacteristic* pStatusCharacteristic;
extern BLECharacteristic* pFillCharacteristic;
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
