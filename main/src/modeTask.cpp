#include <global.h>
#include <Arduino.h>
#include <fftFunctions.h>

unsigned long refreshTimeShiftMatrix;
unsigned long milliSecLastCheckShiftMatrix = 0;

unsigned long refreshTimeDoFFT = 10;
unsigned long milliSecLastCheckDoFFT = 0;



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



void modeTask() {
   // check which mode the hat is in
   if (state == "sound") {
      if (millis() > milliSecLastCheckDoFFT + refreshTimeDoFFT) {
         doFFT();
         updateMatrix();
         milliSecLastCheckDoFFT = millis();
      }  
   } else if (state == "motion") {

   } else if (state == "matrix") {
      int speedInt = speed.toInt(); 
      if (!speedInt) return;

      refreshTimeShiftMatrix = 700 - (abs(speedInt)*200);

      if (millis() > milliSecLastCheckShiftMatrix + refreshTimeShiftMatrix && !updatingMatrix) {
         shiftMatrix(speedInt/abs(speedInt));
         updateMatrix();
         milliSecLastCheckShiftMatrix = millis();
      }  
   }  
}
