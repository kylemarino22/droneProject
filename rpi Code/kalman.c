#include "kalman.h"

void beginKalman(kalman_t *k){

  k->qAngle = 0.001f;
  k->qBias = 0.003f;
  k->rMeasure = 0.03f;


  k->angle = 0.0f;
  k->bias = 0.0f;

  k->P[0][0] = 0.0f;
  k->P[0][1] = 0.0f;
  k->P[1][0] = 0.0f;
  k->P[1][1] = 0.0f;
}

void setAngle(kalman_t *k, float angle){
  k->angle = angle;
}

float getAngle(kalman_t *k, float newAngle, float newRate, float dt){

k->rate = newRate -k->bias;
k->angle += dt *k->rate;

k->P[0][0] += dt * (dt * k->P[1][1] - k->P[0][1] - k->P[1][0] + k->qAngle);
k->P[0][1] -= dt * k->P[1][1];
k->P[1][0] -= dt * k->P[1][1];
k->P[1][1] += k->qBias * dt;

  float S = k->P[0][0] + k->rMeasure;

  float K[2];
  K[0] = k->P[0][0] / S;
  K[1] = k->P[1][0] / S;

  float y = newAngle - k->angle;

k->angle += K[0] * y;
k->bias += K[1] * y;

  float P00_temp = k->P[0][0];
  float P01_temp = k->P[0][1];

k->P[0][0] -= K[0] * P00_temp;
k->P[0][1] -= K[0] * P01_temp;
k->P[1][0] -= K[1] * P00_temp;
k->P[1][1] -= K[1] * P01_temp;

  return k->angle;

}
