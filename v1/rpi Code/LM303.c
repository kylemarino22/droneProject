
#include "LM303.h"
#include<stdio.h>
#include<stdlib.h>
#include<wiringPiI2C.h>
#include<math.h>
#include "kalman.h"




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

   mag_data.x = x;
   mag_data.y = y;
   mag_data.z = z;

}

void LM303_accel_read(){
  unsigned char status;

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

   accel_data.x = 0.001 * 9.80665 * (float)rawx + 0.60;
   accel_data.y = 0.001 * 9.80665 * (float)rawy + 0.33;
   accel_data.z = 0.001 * 9.80665 * (float)rawz - 0.92;

}
