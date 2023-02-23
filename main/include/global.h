#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define LED 2
extern String state;

// Bluetooth utilities

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

extern BLEServer* pServer;
extern BLECharacteristic* pCharacteristic;
extern bool deviceConnected;
extern bool oldDeviceConnected;

class characteristicCallbacks: public BLECharacteristicCallbacks {
   void onWrite(BLECharacteristic *pCharacteristic);
};

class serverCallbacks: public BLEServerCallbacks {
   void onConnect(BLEServer* pServer);
   void onDisconnect(BLEServer* pServer);
};
