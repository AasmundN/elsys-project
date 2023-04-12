#include <fftFunctions.h>
#include <Arduino.h>
#include <ESP32_fft.h>
#include <global.h>

//Sampling
#define FFT_N 256 
#define SAMPLEFREQ 20000  

//FFT
float fft_input[FFT_N];
float fft_output[FFT_N];
ESP_fft FFT(FFT_N, SAMPLEFREQ, FFT_REAL, FFT_FORWARD, fft_input, fft_output);

//LED-index from FFT
#define NUM_LEDS 8
const int max_value = 25000;
const int max_freq = 6000;
const int min_freq = 100;
const int num_of_bands = 19;
const int freq_intervall = max_freq / num_of_bands;
int led_intervall = max_value/NUM_LEDS;

//Index arrays
float amplitude_array[num_of_bands];
int counters[num_of_bands];
int sums[num_of_bands];

void resetCounters(){
   for (int i = 0; i < num_of_bands; ++i) {
      counters[i] = 0;
      sums[i] = 0;
   }
}

void valuesToSum(){
   for (int i = 0; i < FFT_N; ++i) {
      if (fft_output[i] < max_value) {
         switch (static_cast<int>(FFT.frequency(i)))
         {
            case min_freq ... (min_freq + freq_intervall - 1):
               sums[0] += fft_output[i];
               counters[0]++;
               break;

            case (min_freq + freq_intervall) ... (min_freq + 2*freq_intervall- 1):
               sums[1] += fft_output[i];
               counters[1]++;
               break; 
            
            case (min_freq + 2*freq_intervall) ... (min_freq + 3*freq_intervall - 1):
               sums[2] += fft_output[i];
               counters[2]++;
               break; 

            case (min_freq + 3*freq_intervall) ... (min_freq + 4*freq_intervall - 1):
               sums[3] += fft_output[i];
               counters[3]++;
               break; 

            case (min_freq + 4*freq_intervall) ... (min_freq + 5*freq_intervall - 1):
               sums[4] += fft_output[i];
               counters[4]++;
               break;

            case (min_freq + 5*freq_intervall) ... (min_freq + 6*freq_intervall - 1):
               sums[5] += fft_output[i];
               counters[5]++;
               break; 

            case (min_freq + 6*freq_intervall) ... (min_freq + 7*freq_intervall - 1):
               sums[6] += fft_output[i];
               counters[6]++;
               break; 

            case (min_freq + 7*freq_intervall) ... (min_freq + 8*freq_intervall - 1):
               sums[7] += fft_output[i];
               counters[7]++;
               break; 

            case (min_freq + 8*freq_intervall) ... (min_freq + 9*freq_intervall - 1):
               sums[8] += fft_output[i];
               counters[8]++;
               break; 

            case (min_freq + 9*freq_intervall) ... (min_freq + 10*freq_intervall - 1):
               sums[9] += fft_output[i];
               counters[9]++;
               break; 

            case (min_freq + 10*freq_intervall) ... (min_freq + 11*freq_intervall - 1):
               sums[10] += fft_output[i];
               counters[10]++;
               break;           

            case (min_freq + 11*freq_intervall) ... (min_freq + 12*freq_intervall - 1):
               sums[11] += fft_output[i];
               counters[11]++;
               break; 

            case (min_freq + 12*freq_intervall) ... (min_freq + 13*freq_intervall - 1):
               sums[12] += fft_output[i];
               counters[12]++;
               break; 

            case (min_freq + 13*freq_intervall) ... (min_freq + 14*freq_intervall - 1):
               sums[13] += fft_output[i];
               counters[13]++;
               break; 

            case (min_freq + 14*freq_intervall) ... (min_freq + 15*freq_intervall - 1):
               sums[14] += fft_output[i];
               counters[14]++;
               break; 

            case (min_freq + 15*freq_intervall) ... (min_freq + 16*freq_intervall - 1):
               sums[15] += fft_output[i];
               counters[15]++;
               break; 

            case (min_freq + 16*freq_intervall) ... (min_freq + 17*freq_intervall - 1):
               sums[16] += fft_output[i];
               counters[16]++;
               break; 

            case (min_freq + 17*freq_intervall) ... (min_freq + 18*freq_intervall - 1):
               sums[17] += fft_output[i];
               counters[17]++;
               break; 

            case (min_freq + 18*freq_intervall) ... (min_freq + 19*freq_intervall - 1):
               sums[18] += fft_output[i];
               counters[18]++;
               break; 

            case (min_freq + 19*freq_intervall) ... (min_freq + 20*freq_intervall - 1):
               sums[19] += fft_output[i];
               counters[19]++;
               break; 

            default:
               break;
         }
      }
   }
}

void magnitudeToIndex() {
   for (int i = 0; i < num_of_bands; ++i) {
      if (counters[i] != 0){
         amplitude_array[i] = sums[i] / (counters[i]*led_intervall);
      } else {
         amplitude_array[i] = 0;
      }
   }
}

void spectrumToMatrix() {
   for (int columns = 0; columns < num_of_bands; ++columns) {
      for (int rows = 0; rows < ROWS; ++rows) {
         for (int k = 0; k < 3; ++k) {
            if (rows < (NUM_LEDS - static_cast<int>(amplitude_array[columns]))) {
               ledMatrix[rows][columns][k] = 0;
            } else {
               switch (k) {
                  case 0:
                     ledMatrix[rows][columns][k] = (rows*255/7);
                     break;
                  
                  case 1:
                     if (rows <= (7/2)) {
                        ledMatrix[rows][columns][k] = 255*rows/7;
                     } else { 
                        ledMatrix[rows][columns][k] = rows*255/7 - 255;
                     }
                     break; 

                  case 2:
                     ledMatrix[rows][columns][k] = 255 - 255*rows/7;
                     break;
                     
                  default:
                     break;
               }
            }
         }
      }
   }
}

void doFFT() {
    //Reset counters
    resetCounters();

    //Samples data and saves in samples
    int samples_read = sampler->read(samples, SAMPLE_SIZE);

    //Move data to fft_input
    for (int k = 0; k < samples_read; ++k) {
        fft_input[k] = (float)samples[k];
    }

    //Execute transformation
    FFT.removeDC();
    FFT.hammingWindow();
    FFT.execute();
    FFT.complexToMagnitude();

    //Add values to sums
    valuesToSum();

    //Calculate indexes and saves inn amplitude_array
    magnitudeToIndex();

    //Send spectrum with rainbow colors to Matrix
    spectrumToMatrix();
}