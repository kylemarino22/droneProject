#include "gyro.h"
#include<stdio.h>
#include<stdlib.h>
#include<wiringPiI2C.h>
#include<math.h>
#include<stdint.h>


void gyro_begin(){

  if(!wiringPiI2CSetup(gyro_Address)){
     printf("fail to init i2c!");
  }
  gyro_fd = wiringPiI2CSetup(gyro_Address);

  if(wiringPiI2CReadReg8(gyro_fd,0x0F) != 0xD7){
     printf("gyro not connected %x", wiringPiI2CReadReg8(gyro_fd,0x0F));
  }

  wiringPiI2CWriteReg8(gyro_fd, 0x20, 0b00000000);
  wiringPiI2CWriteReg8(gyro_fd, 0x20, 0b00001111);
  wiringPiI2CWriteReg8(gyro_fd, 0x23, 0x10);

  printf("work");

}

void gyro_read(){

uint8_t status;
uint8_t bit_position;
uint8_t mask;
uint8_t bit_status1;
uint8_t bit_status2;

  do {
    bit_position = 3;
    status = wiringPiI2CReadReg8(gyro_fd,0x27);
    mask =  1 << bit_position;
    bit_status1 = status & mask;

    bit_position = 7;
    status = wiringPiI2CReadReg8(gyro_fd,0x27);
    mask =  1 << bit_position;
    bit_status2 = status & mask;

  } while( (bit_status1 != 0b00001000) || (bit_status2 != 0b10000000));

  uint8_t  lo_x = wiringPiI2CReadReg8(gyro_fd,0x28);
  uint8_t hi_x = wiringPiI2CReadReg8(gyro_fd,0x29);

  uint8_t lo_y = wiringPiI2CReadReg8(gyro_fd,0x2A);
  uint8_t hi_y = wiringPiI2CReadReg8(gyro_fd,0x2B);

  uint8_t lo_z = wiringPiI2CReadReg8(gyro_fd,0x2C);
  uint8_t hi_z = wiringPiI2CReadReg8(gyro_fd,0x2D);

  int16_t rawx = (int16_t) ((lo_x) | (hi_x << 8));
  int16_t rawy = (int16_t) ((lo_y) | (hi_y << 8));
  int16_t rawz = (int16_t) ((lo_z) | (hi_z << 8));

  gyro_data.x = rawx;
  gyro_data.y = rawy;
  gyro_data.z = rawz;
}

void gyroCalibrate(){
  float tempX = 0;
  float tempY = 0;
  float tempZ = 0;
  int i;
  printf("Calibrating\n");
  for (i = 0; i < 500; i++) {
    if(i % 100 == 0){
      printf(".\n");
    }
    gyro_read();
    tempX += gyro_data.x;
    tempY += gyro_data.y;
    tempZ += gyro_data.z;
    delay(3);
  }
  gyro_calibrate.x = tempX/500;
  gyro_calibrate.x = tempX/500;
  gyro_calibrate.x = tempX/500;
}

void gyroCalculate(float dt){
  float testX = (gyro_data.x - gyro_calibrate.x)  * 0.0175;
  float testY = (gyro_data.y - gyro_calibrate.y)  * 0.0175;
  float testZ = (gyro_data.z - gyro_calibrate.z)  * 0.0175;

  if (!(testZ < 0.05 && testZ > -0.05)) {
    gyro_data.z += testZ * dt;
  }
  else {
    testZ = 0;
  }

  if (!(testY < 0.05 && testY > -0.05)) {
    gyro_data.y += testY * dt;
  }
  else {
    testY = 0;
  }

  if (!(testX < 0.05 && testX > -0.05)) {
    gyro_data.x += testX * dt;
  }
  else {
    testX = 0;
  }

  gyro_data.x += gyro_data.y * sin(testZ * (3.1415 / 180));
  gyro_data.y -= gyro_data.x * sin(testZ * (3.1415 / 180));

  gyro_rate = (XYZ){testX, testY, testZ};
}
