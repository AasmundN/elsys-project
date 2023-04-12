#include <global.h>
#include <Arduino.h>


   double getFillPointZ(int n, const double radius, const double height, double a, double b) {
      double alpha = (((2*PI))/(COLS-1))*n;
      double xPos = sin(alpha)*radius;
      double yPos = cos(alpha)*radius;

      double fillHeight = (height/2) * ((10 - fill.toDouble())/10);

      double z = a*xPos + b*yPos + fillHeight;

      double fillToLed = z;
      return z;
   }

   void fillLedPoint(int n, double fillToLed) {
      for(int y = ROWS-1; y >= 0; y--){
         if (y > fillToLed) {
            ledMatrix[y][n][0] = 255;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 0; 
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

   double a = tan(angleX);
   double b = tan(angleY); 

   
   // Beer magic
   for(int n = 0; n < COLS; n++){ 

      double fillToLed = getFillPointZ(n, radius, height, a, b);

      for(int y = ROWS-1; y >= 0; y--){
         if (y > fillToLed) {
            ledMatrix[y][n][0] = 255;
            ledMatrix[y][n][1] = 120;
            ledMatrix[y][n][2] = 0; 
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
}