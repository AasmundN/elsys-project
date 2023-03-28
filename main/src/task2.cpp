#include <global.h>
#include <ESP32_fft.h>

unsigned long milliSecLastCheckShiftMatrix = 0;

unsigned long refreshTimeDoFFT = 30;
unsigned long milliSecLastCheckDoFFT = 0;
//Definer FFT-variabler her

void shiftMatrix(int direction) {
   // create new, shifted matrix
   uint8_t newMatrix[ROWS][COLS][3];

   if (direction > 0) { // shift right
      for (int row = 0; row < ROWS; row++) {
         for (int col = 0; col < COLS; col++) {
            for (int k = 0; k<3; k++) {
               if (col == COLS-1) {
                  newMatrix[row][0][k] = ledMatrix[row][col][k];
               }
               else { newMatrix[row][col+1][k] = ledMatrix[row][col][k];
               }
            }
         }
      }
   } else { // shift left
      for (int row = 0; row < ROWS; row++) {
         for (int col = 0; col < COLS; col++) {
            for (int k = 0; k<3; k++) {
               if (col == 0) {
                  newMatrix[row][COLS-1][k] = ledMatrix[row][col][k];
               } else { 
                  newMatrix[row][col-1][k] = ledMatrix[row][col][k];
               }
            }
         }
      }
   }

   // update old matrix
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         for (int k = 0; k<3; k++) {
            ledMatrix[row][col][k] = newMatrix[row][col][k];
         }
      }
   }
}

void readSoundData() {

}

void doFFT() {
   
}



void task2() {
   // check which mode the hat is in
   if (state == "sound") {
      if (millis() > milliSecLastCheckDoFFT + refreshTimeDoFFT) {
         doFFT();
         milliSecLastCheckDoFFT = millis();
      }  
   } else if (state == "motion") {

   } else if (state == "matrix") {
      int speedInt = speed.toInt(); 
      if ((millis() > (milliSecLastCheckShiftMatrix+600-(abs(speedInt)*100))) && (speedInt != 0)) {
         shiftMatrix(speedInt/abs(speedInt));
         milliSecLastCheckShiftMatrix = millis();
      }
   } 
}
