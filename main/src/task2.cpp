#include <global.h>
#include <ESP32_fft.h>

unsigned long refreshTimeShiftMatrix;
unsigned long milliSecLastCheckShiftMatrix = 0;

unsigned long refreshTimeDoFFT = 100;
unsigned long milliSecLastCheckDoFFT = 0;

unsigned long refreshTimeDoMotion = 10;
unsigned long milliSecLastCheckDoMotion = 0;

//Sampling
#define FFT_N 256 // Hvis høyere tar samplingen for lang tid 
#define TOTAL_TIME 0.013 //The time in which data was captured. This is equal to FFT_N/sampling_freq. DOBBELTSJEKK OM DENNE SKAL STÅ I s
#define SAMPLEFREQ FFT_N/TOTAL_TIME  //19692Hz
#define AUDIO_IN_PIN 35 
float fft_signal[FFT_N];

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
   // sample read test
   int samples_read = sampler->read(samples, SAMPLE_SIZE);
   Serial.println(samples[0]);
}

// Forsøk på å lage gyrodatabehandler-funk
   void getAnglesGyro(int angles[2]) {
      int16_t AcX,AcY,AcZ;

      int minVal=265;
      int maxVal=402;

      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr,14,1);

      AcX=Wire.read()<<8|Wire.read();
      AcY=Wire.read()<<8|Wire.read();
      AcZ=Wire.read()<<8|Wire.read();

      int xAng = map(AcX,minVal,maxVal,-90,90);
      int yAng = map(AcY,minVal,maxVal,-90,90);
      int zAng = map(AcZ,minVal,maxVal,-90,90);
      
      double angleX=(atan2(-yAng, -zAng)+PI);
      double angleY=(atan2(-xAng, -zAng)+PI);

      angles[0] = angleX;
      angles[1] = angleY;
   }

   double getFillPointZ(int n, const double radius, const double height, double a, double b) {
      double alpha = (((2*PI))/(COLS-1))*n;
      double xPos = sin(alpha)*radius;
      double yPos = cos(alpha)*radius;

      double z = a*xPos + b*yPos + height/4;

      double fillToLed = z;
      return z;
   }

   void fillLedPoint(int n, double fillToLed) {
      for(int y = ROWS-1; y >= 0; y--){
         if (y > fillToLed) {
            ledMatrix[y][n][0] = 255;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 0; 
            //leds[ledMatrixIndices[y][n]] = CRGB(0, 0, 0);
         }
         else if (y == static_cast<int>(fillToLed)){
            ledMatrix[y][n][0] = 200;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 50; 
         }
         else {
            ledMatrix[y][n][0] = 0;
            ledMatrix[y][n][1] = 0;
            ledMatrix[y][n][2] = 0; 
         }
      }     
   }

void doMotion() {
   const double radius = 8.75;
   const double height = 13.5;

   int angles[2];

   getAnglesGyro(angles);   

   double angleX = angles[0];
   double angleY = angles[1];

   double a = tan(angleX);
   double b = tan(angleY);

   
   // Beer magic
   for(int n = 0; n < COLS; n++){ /*
      double alpha = (((2*PI))/(COLS-1))*n;
      double xPos = sin(alpha)*radius;
      double yPos = cos(alpha)*radius;

      double z = a*xPos + b*yPos + height/4;

      double fillToLed = z; */

      double fillToLed = getFillPointZ(n, radius, height, a, b);

      fillLedPoint(n, fillToLed); /*
      for(int y = ROWS-1; y >= 0; y--){
         if (y > fillToLed) {
            ledMatrix[y][n][0] = 255;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 0; 
            //leds[ledMatrixIndices[y][n]] = CRGB(0, 0, 0);
         }
         else if (y == static_cast<int>(fillToLed)){
            ledMatrix[y][n][0] = 200;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 50; 
         }
         else {
            ledMatrix[y][n][0] = 0;
            ledMatrix[y][n][1] = 0;
            ledMatrix[y][n][2] = 0; 
         }
      }*/
   FastLED.show();
   }
}


void task2() {
   // check which mode the hat is in
   if (state == "sound") {
      if (millis() > milliSecLastCheckDoFFT + refreshTimeDoFFT) {
         doFFT();
         milliSecLastCheckDoFFT = millis();
      }  
   } else if (state == "motion") {
      if (millis() > milliSecLastCheckDoMotion + refreshTimeDoMotion) {
         doMotion();
         updateMatrix();
         milliSecLastCheckDoMotion = millis();
      } 
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
