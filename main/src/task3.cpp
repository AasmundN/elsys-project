#include <global.h>

unsigned long milliSecLastCheck = 0;
unsigned long refreshTime = 500;

void updateMatrix() {
   //Kode som oppdaterer led matrise
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         // get new LED color values
         uint8_t red = matrixArray[row][col][0];
         uint8_t green = matrixArray[row][col][1];
         uint8_t blue = matrixArray[row][col][2];

         // update LED color values
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