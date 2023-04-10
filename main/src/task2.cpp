#include <global.h>
#include <ESP32_fft.h>

unsigned long refreshTimeShiftMatrix;
unsigned long milliSecLastCheckShiftMatrix = 0;

unsigned long refreshTimeDoFFT = 100;
unsigned long milliSecLastCheckDoFFT = 0;

//Sampling
#define FFT_N 256 // Hvis høyere tar samplingen for lang tid 
#define SAMPLEFREQ 20000  

//FFT
float fft_input[FFT_N];
float fft_output[FFT_N];
ESP_fft FFT(FFT_N, SAMPLEFREQ, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

//LED-index from FFT
#define NUM_LEDS 30;
const int max_value = 28000;
int led_intervall = max_value/NUM_LEDS;
const int num_of_bands = 8;

float amplitude_array[num_of_bands];
int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0, k7 = 0, k8 = 0;
int sum_band_1 = 0, sum_band_2 = 0, sum_band_3 = 0, sum_band_4 = 0, sum_band_5 = 0, sum_band_6 = 0, sum_band_7 = 0, sum_band_8  = 0;


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

void doFFT() {

   //Resetter counters
   k1 = 0; k2 = 0; k3 = 0; k4 = 0; k5 = 0; k6 = 0; k7 = 0; k8 = 0;
   sum_band_1 = 0; sum_band_2 = 0; sum_band_3 = 0; sum_band_4 = 0; sum_band_5 = 0; sum_band_6 = 0; sum_band_7 = 0; sum_band_8 = 0;

   //Sampler data og lagrer i fft_signal
   int samples_read = sampler->read(samples, SAMPLE_SIZE);

   //Flytter data til fft_input
   for (int k = 0; k < samples_read; ++k) {
      fft_input[k] = (float)samples[k];
   }

   // Execute transformation
   FFT.removeDC();
   FFT.hammingWindow();
   FFT.execute();
   FFT.complexToMagnitude();


   for (int i = 0; i < FFT_N; ++i) {
      if (fft_output[i] < 28000) {
         if (FFT.frequency(i) >= 100 && FFT.frequency(i) < 900) {
            sum_band_1 += fft_output[i];
            k1++;
         } 
         else if (FFT.frequency(i) >= 900 && FFT.frequency(i) < 1700) {
            sum_band_2 += fft_output[i];
            k2++;
         } 
         else if (FFT.frequency(i) >= 1700 && FFT.frequency(i) < 2500) {
            sum_band_3 += fft_output[i];
            k3++;
         }
         else if (FFT.frequency(i) >= 2500 && FFT.frequency(i) < 3300) {
            sum_band_4 += fft_output[i];
            k4++;
         }
         else if (FFT.frequency(i) >= 3300 && FFT.frequency(i) < 4100) {
            sum_band_5 += fft_output[i];
            k5++;
         }
         else if (FFT.frequency(i) >= 4100 && FFT.frequency(i) < 4900) {
            sum_band_6 += fft_output[i];
            k6++;
         }
         else if (FFT.frequency(i) >= 4900 && FFT.frequency(i) < 5700) {
            sum_band_7 += fft_output[i];
            k7++;
         }
         else if (FFT.frequency(i) >= 5700 && FFT.frequency(i) < 6500) {
            sum_band_8 += fft_output[i]; 
            k8++;
         }
      }
   }

   //amplitude_array inneholder (kanskje) max_index
   if (k1 != 0) {
      amplitude_array[0] = sum_band_1 / (k1*led_intervall); 
   } else {
      amplitude_array[0] = 0; 
   }

   if (k2 != 0) {
      amplitude_array[1] = sum_band_2 / (k2*led_intervall);
   } else {
      amplitude_array[1] = 0;
   }

   if (k3 != 0) {
      amplitude_array[2] = sum_band_3 / (k3*led_intervall);
   } else {
      amplitude_array[2] = 0;
   }

   if (k4 != 0) {
      amplitude_array[3] = sum_band_4 / (k4*led_intervall);
   } else {
      amplitude_array[3] = 0;
   }


   if (k5 != 0) {
      amplitude_array[4] = sum_band_5 / (k5*led_intervall);
   } else {
      amplitude_array[4] = 0;   
   }


   if (k6 != 0) {
      amplitude_array[5] = sum_band_6 / (k6*led_intervall);
   } else {
      amplitude_array[5] = 0;
   }
   
   if (k7 != 0) {
      amplitude_array[6] = sum_band_7 / (k7*led_intervall);
   } else {
      amplitude_array[6] = 0;
   }

   if (k8 != 0) {
      amplitude_array[7] = sum_band_8 / (k8*led_intervall); 
   } else {
      amplitude_array[7] = 0;
   }



   Serial.print(samples_read); Serial.print(" ------- ");

   Serial.print(amplitude_array[0]); Serial.print("; ");
   Serial.print(amplitude_array[2]); Serial.print("; ");
   Serial.print(amplitude_array[2]); Serial.print("; ");
   Serial.print(amplitude_array[3]); Serial.print("; ");
   Serial.print(amplitude_array[4]); Serial.print("; ");
   Serial.print(amplitude_array[5]); Serial.print("; ");
   Serial.print(amplitude_array[6]); Serial.print("; ");
   Serial.print(amplitude_array[7]); Serial.println("; ");


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
      if (!speedInt) return;

      refreshTimeShiftMatrix = 700 - (abs(speedInt)*200);

      if (millis() > milliSecLastCheckShiftMatrix + refreshTimeShiftMatrix && !updatingMatrix) {
         shiftMatrix(speedInt/abs(speedInt));
         updateMatrix();
         milliSecLastCheckShiftMatrix = millis();
      }  
   } 
}
