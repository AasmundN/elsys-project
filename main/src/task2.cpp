#include <global.h>

unsigned long milliSecLastCheckShiftMatrix = 0;

void shiftMatrix() {
   // create new, shifted matrix
   uint8_t newMatrix[ROWS][COLS][3];
   
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         for (int k = 0; k<3; k++){
            newMatrix[row][col+1][k] = ledMatrix[row][col][k];
            if (col == COLS-1) {
               newMatrix[row][0][k] = ledMatrix[row][col][k];
            }
         }
      }
   }
   // update old matrix
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         for (int k = 0; k<3; k++){
            ledMatrix[row][col][k] = newMatrix[row][col][k];
         }
      }
   }
}

void task2() {
   // check which mode the hat is in
   if (state == "sound") {
      
   } else if (state == "motion") {

   } else if (state == "matrix") {
      int speedInt = speed.toInt(); 
      if ((millis() > (milliSecLastCheckShiftMatrix+600-speedInt*100)) && (speedInt > 0)) {
         shiftMatrix();
         milliSecLastCheckShiftMatrix = millis();
      }
   } 
}
