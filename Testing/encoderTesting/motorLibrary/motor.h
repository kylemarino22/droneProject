
#ifndef _motor_h_
#define _motor_h_

#include "Arduino.h"
#include <Servo.h>

typedef struct motor_t {
        Servo motor;
        float calibration;
} motor_t;

extern motor_t motorArray[4];
extern motor_t motor1, motor2, motor3, motor4;
extern long looptimer;

void motorInitialize();
void changeSpeed(int,int,int,int);
void motorInitializev2();

void motorUpdate(int, int, int, int);


#endif
