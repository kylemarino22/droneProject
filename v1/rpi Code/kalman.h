#ifndef _kalman_H_
#define _kalman_H_
#include "struct.h"



void beginKalman(kalman_t *k);
void setAngle(kalman_t *k, float angle);
float getAngle(kalman_t *k, float newAngle, float newRate, float dt);

#endif
