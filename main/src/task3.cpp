#include <Arduino.h>

#include <global.h>

int milliSecLastCheck = 0;
int refreshTime = 500;

void updateMatrix() {
    //Kode som oppdaterer led matrise
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < cols; col++) {
        uint8_t red = matrixArray[row][col][0];
        uint8_t green = matrixArray[row][col][1];
        uint8_t blue = matrixArray[row][col][2];
        
        leds[ledMatrix[row][col]] = CRGB(red, green, blue);
      }
   }

   FastLED.show();
}

void task3() {
    if (millis() > milliSecLastCheck + refreshTime) {
        updateMatrix();
        milliSecLastCheck = millis();
    }  
}