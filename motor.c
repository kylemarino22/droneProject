#include "motor.h"
#include<stdio.h>
#include<stdlib.h>
#include<pigpio.h>
#include<math.h>
#include<stdint.h>

unsigned motorArray[4] = {17, 18, 23, 24};
int i;



void motor_initialize(){
  gpioInitialise();

  delay(1000);
  for(i = 0; i < 4; i++){
    gpioServo(motorArray[i], 2000);
    delay (3000);
    gpioServo(motorArray[i], 1000);
    delay (3000);
  }
  delay(1000);
}

void motor_update(int m1, int m2, int m3, int m4){
  int mArray[4] = {m1, m2, m3, m4};

  _motor_data.m1 = mArray[0];
  _motor_data.m2 = mArray[1];
  _motor_data.m3 = mArray[2];
  _motor_data.m4 = mArray[3];
  for(i = 0; i < 4; i++){
    gpioServo(motorArray[i], mArray[i]);
  }
}

void motor_change(int m1, int m2, int m3, int m4){
  _motor_data.m1 += m1;
  _motor_data.m2 += m2;
  _motor_data.m3 += m3;
  _motor_data.m4 += m4;
  gpioServo(motorArray[0], _motor_data.m1);
  gpioServo(motorArray[1], _motor_data.m2);
  gpioServo(motorArray[2], _motor_data.m3);
  gpioServo(motorArray[3], _motor_data.m4);
}
