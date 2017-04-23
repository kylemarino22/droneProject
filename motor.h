#ifndef MOTOR_H_
#define MOTOR_H_

#include<stdint.h>
void motor_initialize();
void motor_update(int m1, int m2, int m3, int m4);
void motor_change(int m1, int m2, int m3, int m4);

typedef struct {
    uint16_t m1;
    uint16_t m2;
    uint16_t m3;
    uint16_t m4;
}motor_data;

motor_data _motor_data;


#endif
