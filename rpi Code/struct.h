#ifndef _struct_h_
#define _struct_h_

#define M_PI 3.14159265358s

typedef struct {
  float x;
  float y;
  float z;
}XYZ;

typedef struct {

  float qAngle;
  float qBias;
  float rMeasure;

  float angle;
  float bias;
  float rate;

  float P[2][2];

} kalman_t;

typedef struct {
  float Xdata[6];
  float Ydata[6];
  float Zdata[6];

} sharpChange_t;

sharpChange_t accelSCF;
sharpChange_t magSCF;

#endif
