#include <global.h>
#include <task2.h>

unsigned long milliSecLastCheck = 0;

void shiftMatrix(){
   //Kode som flytter rader en rad bortover
   uint8_t newMatrix[ROWS][COLS][3];
   // = ledMatrix[ROWS][COLS][3];
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
   for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
         for (int k = 0; k<3; k++){
            ledMatrix[row][col][k] = newMatrix[row][col][k];
         }
      }
   }
}


void task2() {
   if (state == "sound")
      digitalWrite(LED, HIGH);
   else if (state == "matrix") {
      
      int speedInt = speed.toInt() * 100; 
      if ((millis() > (milliSecLastCheck + 600 - speedInt)) && (speedInt > 0)) {
         shiftMatrix();
         milliSecLastCheck = millis();
      }
   } else
      digitalWrite(LED, LOW);  
}
