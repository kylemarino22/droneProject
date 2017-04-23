
#include "LM303.h"
#include<stdio.h>
#include<stdlib.h>
#include<wiringPiI2C.h>
#include<math.h>

float PI = 3.14159265;

int LM303_mag_begin(){
   if(!wiringPiI2CSetup(LM303_mag_Address)){
      printf("fail to init i2c!");
   }
   LM303_mag_fd = wiringPiI2CSetup(LM303_mag_Address);

   wiringPiI2CWriteReg8(LM303_mag_fd, 0x00, 0b00011100);
   wiringPiI2CWriteReg8(LM303_mag_fd, 0x01, 0b10000000);
   wiringPiI2CWriteReg8(LM303_mag_fd, 0x02, 0x00);

   if(wiringPiI2CReadReg8(LM303_mag_fd,0x00) != 0x10){
      printf("BMP not connected %x", wiringPiI2CReadReg8(LM303_mag_fd,0x00));
   }
   printf("work");

}




int LM303_accel_begin(){
   if(!wiringPiI2CSetup(LM303_accel_Address)){
      printf("fail to init i2c!");
   }
   LM303_accel_fd = wiringPiI2CSetup(LM303_accel_Address);

   wiringPiI2CWriteReg8(LM303_accel_fd, 0x20, 0x97);
   wiringPiI2CWriteReg8(LM303_accel_fd, 0x23, 0x88);

   delay(50);

   if(wiringPiI2CReadReg8(LM303_accel_fd, 0x20) != 0x57){
      printf("BMP not connected %x", wiringPiI2CReadReg8(LM303_mag_fd,0x00));
   }

   printf("work");
}

void LM303_mag_read(){

   int hi_x = wiringPiI2CReadReg8(LM303_mag_fd,0x03);
   int lo_x = wiringPiI2CReadReg8(LM303_mag_fd,0x04);
   int hi_z = wiringPiI2CReadReg8(LM303_mag_fd,0x05);
   int lo_z = wiringPiI2CReadReg8(LM303_mag_fd,0x06);
   int hi_y = wiringPiI2CReadReg8(LM303_mag_fd,0x07);
   int lo_y = wiringPiI2CReadReg8(LM303_mag_fd,0x08);

   int16_t x = (int16_t)((uint16_t)lo_x | ((uint16_t)hi_x << 8));
   int16_t z = (int16_t)((uint16_t)lo_z | ((uint16_t)hi_z << 8));
   int16_t y = (int16_t)((uint16_t)lo_y | ((uint16_t)hi_y << 8));

   _mag_data.x = x;
   _mag_data.y = y;
   _mag_data.z = z;
  //  float pitch;
  //  float roll;
   //
  //  roll = (float)atan2(_accel_data.y, _accel_data.z);
   //
  //  if (_accel_data.y * sin(roll) + _accel_data.z * cos(roll) == 0){
  //    pitch = _accel_data.x > 0 ? (PI / 2) : (-PI / 2);
  //  }
  //  else{
  //    pitch = (float)atan(-_accel_data.x / (_accel_data.y * sin(roll) + \
  //                                                                    _accel_data.z * cos(roll)));
  //  }
   //
  //  float heading = (float)atan2(_mag_data.z * sin(roll) - _mag_data.y * cos(roll), \
  //                                     _mag_data.x * cos(pitch) + \
  //                                     _mag_data.y * sin(pitch) * sin(roll) + \
  //                                     _mag_data.z * sin(pitch) * cos(roll));
   //
  //   _fusion_data.roll = roll * 180/PI;
  //   _fusion_data.pitch = pitch * 180 / PI;
  //   _fusion_data.heading = heading * 180 / PI;





}

void LM303_accel_read(){
  unsigned char status;

   //  if(wiringPiI2CReadReg8(LM303_accel_fd, 0x20) != 0x57){
   //     printf("BMP not connected %x", wiringPiI2CReadReg8(LM303_mag_fd,0x00));
   //  }
   // LM303_accel_fd = wiringPiI2CSetup(LM303_accel_Address);

   do {
     status = wiringPiI2CReadReg8(LM303_accel_fd,0x27);
   } while (status == 0);

   uint8_t xlo = wiringPiI2CReadReg8(LM303_accel_fd,0x28);
   uint16_t xhi = wiringPiI2CReadReg8(LM303_accel_fd,0x29);

   uint8_t ylo = wiringPiI2CReadReg8(LM303_accel_fd,0x2A);
   uint8_t yhi = wiringPiI2CReadReg8(LM303_accel_fd,0x2B);

   uint8_t zlo = wiringPiI2CReadReg8(LM303_accel_fd,0x2C);
   uint8_t zhi = wiringPiI2CReadReg8(LM303_accel_fd,0x2D);

   int16_t rawx = ((int16_t)(xlo | (xhi << 8))) >> 4;
   int16_t rawy = ((int16_t)(ylo | (yhi << 8))) >> 4;
   int16_t rawz = ((int16_t)(zlo | (zhi << 8))) >> 4;

   _accel_data.x = 0.001 * 9.80665 * (float)rawx + 0.60;
   _accel_data.y = 0.001 * 9.80665 * (float)rawy + 0.33;
   _accel_data.z = 0.001 * 9.80665 * (float)rawz - 0.92;

}

void fusionCalc(){
  float pitch;
  float roll;

  roll = (float)atan2(_accel_data.y, _accel_data.z);

  if (_accel_data.y * sin(roll) + _accel_data.z * cos(roll) == 0){
    pitch = _accel_data.x > 0 ? (PI / 2) : (-PI / 2);
  }
  else{
    pitch = (float)atan(-_accel_data.x / (_accel_data.y * sin(roll) + \
                                                                    _accel_data.z * cos(roll)));
  }

  float heading = (float)atan2(_mag_data.z * sin(roll) - _mag_data.y * cos(roll), \
                                     _mag_data.x * cos(pitch) + \
                                     _mag_data.y * sin(pitch) * sin(roll) + \
                                     _mag_data.z * sin(pitch) * cos(roll));

   _fusion_data.roll = roll * 180/PI;
   _fusion_data.pitch = pitch * 180 / PI;
   _fusion_data.heading = heading * 180 / PI;

}

float meanTrimmer(float input[20]){
  int i;
  int j;
  float temp;
  float output;
  output = 0;

  for(i = 0; i < 19; i++){

    for(j = 0; j < 19 - i; j++){
      if(input[j] > input[j+1]){
        temp = input[j+1];
        input[j+1] = input[j];
        input[j] = temp;
      }
    }
  }

  for (i = 0; i < 12; i++){
     output += input[i + 4];
  }
  output /= 12;

  return output;

}

float average(float input[40]){
  float temp = 0;
  int i;
  for(i = 0; i<24; i++){
    temp += input[i];
  }

  temp /= 24;

  return temp;
}

// void accelCorrect(){
//
//   float xArray[3];
//   float yArray[3];
//   float zArray[3];
//   float xtemp[3];
//   float ytemp[3];
//   float ztemp[3];
//   int correctArray[3];
//   int pass;
//   int i;
//
//   for (i = 0; i < 3; i++){
//     LM303_accel_read();
//     xArray[i] = _accel_data.x;
//     yArray[i] = _accel_data.y;
//     zArray[i] = _accel_data.z;
//
//   }
//
//   do {
//
//     pass = 1;
//
//     xtemp[0] = ((2 * xArray[2]) / (xArray[0] + xArray[1])) - 1;
//     xtemp[1] = ((2 * xArray[0]) / (xArray[1] + xArray[2])) - 1;
//     xtemp[2] = ((2 * xArray[1]) / (xArray[2] + xArray[0])) - 1;
//
//     ytemp[0] = ((2 * yArray[2]) / (yArray[0] + yArray[1])) - 1;
//     printf("temp1 %f\n", ytemp[0]);
//     ytemp[1] = ((2 * yArray[0]) / (yArray[1] + yArray[2])) - 1;
//     printf("temp2 %f\n", ytemp[1]);
//     ytemp[2] = ((2 * yArray[1]) / (yArray[2] + yArray[0])) - 1;
//     printf("temp3 %f\n", ytemp[2]);
//
//     ztemp[0] = ((2 * zArray[2]) / (zArray[0] + zArray[1])) - 1;
//     ztemp[1] = ((2 * zArray[0]) / (zArray[1] + zArray[2])) - 1;
//     ztemp[2] = ((2 * zArray[1]) / (zArray[2] + zArray[0])) - 1;
//
//     if(xtemp[0] < 0) xtemp[0] *= -1;
//     if(xtemp[1] < 0) xtemp[1] *= -1;
//     if(xtemp[2] < 0) xtemp[2] *= -1;
//
//     if(ytemp[0] < 0) ytemp[0] *= -1;
//     if(ytemp[1] < 0) ytemp[1] *= -1;
//     if(ytemp[2] < 0) ytemp[2] *= -1;
//
//     if(ztemp[0] < 0) ztemp[0] *= -1;
//     if(ztemp[1] < 0) ztemp[1] *= -1;
//     if(ztemp[2] < 0) ztemp[2] *= -1;
//
//     if(!(xtemp[0] < (1 / xtemp[0])  && xtemp[1] < (1 / xtemp[1])
//     && xtemp[2] <  (1 / xtemp[2]) )){
//       pass = 0;
//     }
//     if(!(ytemp[0] < (1 / ytemp[0])  && ytemp[1] < (1 / ytemp[1])
//     && ytemp[2] < (1 / ytemp[2]))){
//       pass = 0;
//     }
//     if(!(ztemp[0] < (1 / ztemp[0])  && ztemp[1] < (1 / ztemp[1])
//     && ztemp[2] < (2 / ztemp[2]))){
//       pass = 0;
//     }
//
//     if(pass == 0){
//       //x correction
//       if((xtemp[0] < xtemp[1]) && (xtemp[2] < xtemp[1])){
//         correctArray[0] = 1;
//       }
//       else if((xtemp[0] < xtemp[2]) && (xtemp[1] < xtemp[2])){
//         correctArray[0] = 2;
//       }
//       else {
//         correctArray[0] = 0;
//       }
//
//       //y correction
//       if((ytemp[0] < ytemp[1]) && (ytemp[2] < ytemp[1])){
//         correctArray[1] = 1;
//       }
//       else if((ytemp[0] < ytemp[2]) && (ytemp[1] < ytemp[2])){
//         correctArray[1] = 2;
//       }
//       else {
//         correctArray[1] = 0;
//       }
//
//       // z correction
//       if((ztemp[0] < ztemp[1]) && (ztemp[2] < ztemp[1])){
//         correctArray[2] = 1;
//       }
//       else if((ztemp[0] < ztemp[2]) && (ztemp[1] < ztemp[2])){
//         correctArray[2] = 2;
//       }
//       else {
//         correctArray[2] = 0;
//       }
//
//       LM303_accel_read();
//       xArray[correctArray[0]] = _accel_data.x;
//       yArray[correctArray[1]] = _accel_data.y;
//       zArray[correctArray[2]] = _accel_data.z;
//
//     }
//
//   } while(pass == 0);
//
// _accel_data.x = (xArray[0] + xArray[1] + xArray[2]) / 3;
// _accel_data.y = (yArray[0] + yArray[1] + yArray[2]) / 3;
// _accel_data.z = (zArray[0] + zArray[1] + zArray[2]) / 3;
// }


float headingCorrect(float finalheading){

  if (finalheading < 0 ) {
    finalheading *= -1;
    finalheading = -0.0051448 * (pow(finalheading, 2)) + (1.9253 * finalheading) + 0.26076;
    finalheading *= -1;
  }
  else{
    finalheading = -0.0051448 * (pow(finalheading, 2)) + (1.9253 * finalheading) + 0.26076;
  }

  return finalheading;

}

float rollCorrect(float rollArray[40]){
  int i;
  int j;
  float temp;
  float finalRoll;

  for(i = 0; i < 39; i++){

    for(j = 0; j < 39 - i; j++){
      if(rollArray[j] > rollArray[j+1]){
        temp = rollArray[j+1];
        rollArray[j+1] = rollArray[j];
        rollArray[j] = temp;
      }
    }
  }

  for (i = 0; i < 24; i++){
     finalRoll += rollArray[i + 2];
  }
  finalRoll /= 24;

  return finalRoll;

}

float pitchCorrect(float pitchArray[40]){
  int i;
  int j;
  float temp;
  float finalPitch;

  for(i = 0; i < 39; i++){

    for(j = 0; j < 39 - i; j++){
      if(pitchArray[j] > pitchArray[j+1]){
        temp = pitchArray[j+1];
        pitchArray[j+1] = pitchArray[j];
        pitchArray[j] = temp;
      }
    }
  }

  for (i = 0; i < 24; i++){
     finalPitch += pitchArray[i + 2];
  }
  finalPitch /= 24;
  finalPitch += 2;

  return finalPitch;

}
