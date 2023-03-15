#include <global.h>

unsigned long milliSecLastCheck = 0;
unsigned long refreshTime = 500;

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

void task3() {
   if (millis() > milliSecLastCheck + refreshTime) {
      updateMatrix();
      milliSecLastCheck = millis();
   }  
}