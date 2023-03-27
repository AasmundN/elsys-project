#include <global.h>
#include <ESP32_fft.h>

unsigned long milliSecLastCheckShiftMatrix = 0;

unsigned long refreshTimeDoFFT = 30;
unsigned long milliSecLastCheckDoFFT = 0;

//Definer FFT-variabler her
//Sampling
#define FFT_N 256 // Hvis høyere tar samplingen for lang tid 
#define TOTAL_TIME 0.013 //The time in which data was captured. This is equal to FFT_N/sampling_freq. DOBBELTSJEKK OM DENNE SKAL STÅ I s
#define SAMPLEFREQ FFT_N/TOTAL_TIME  //19692Hz > 13000Hz, kan sample frekvenser opp til 1.8kHz
#define AUDIO_IN_PIN 35 //The time in which data was captured. This is equal to FFT_N/sampling_freq
float fft_signal[FFT_N];

//LED-index
#define NUM_LEDS 30;
const int max_value = 28000;
int led_intervall = max_value/NUM_LEDS;
const int num_of_bands = 8;

float amplitude_array[num_of_bands];
int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, k7 = 0, k8 = 0;
int sum_band_1 = 0, sum_band_2 = 0, sum_band_3 = 0, sum_band_4 = 0, sum_band_5 = 0, sum_band_6 = 0, sum_band_7 = 0, sum_band_8  = 0;



void shiftMatrix(int diraction) {
   // create new, shifted matrix
   uint8_t newMatrix[ROWS][COLS][3];
   if (diraction > 0) { // shift right
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
               }
               else {newMatrix[row][col-1][k] = ledMatrix[row][col][k];
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
