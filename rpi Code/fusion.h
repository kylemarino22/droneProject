#ifndef _fusion_h_
#define _fusion_h_
#include <time.h>
#include "struct.h"
#include "kalman.h"

clock_t lastRead, currentRead;
float deltaT;

XYZ currentAngle;
struct timespec current, previous;

void calculateFusion(kalman_t*, kalman_t*);

float calculateTime();
float computeRoll();
float computePitch(float roll);

void sharpChangeFilter(XYZ*, sharpChange_t*);





#endif
