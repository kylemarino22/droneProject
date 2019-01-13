#include "fusion.h"
#include "gyro.h"
#include "LM303.h"
#include <stdio.h>
#include <math.h>
#include "kalman.h"


uint8_t waitOnce = 0;

float calculateTime(){
        //calculates the time between its function call

        clock_gettime(CLOCK_REALTIME, &current);

        float accum = ( current.tv_sec - previous.tv_sec )
                      + ( current.tv_nsec - previous.tv_nsec )
                      / 1000000000.0f;

        previous = current;
        return accum;
}

float computeRoll(){
        return atan2(accel_data.y, accel_data.z);
}

float computePitch(float roll){
        float rollSin = sin(roll);
        float rollCos = cos(roll);
        return atan2(-accel_data.x, accel_data.y * rollSin + accel_data.z * rollCos);
}


void sharpChangeFilter(XYZ *input, sharpChange_t *SCF){
        int i;
        float averageX = 0;
        float averageY = 0;
        float averageZ = 0;
        float bias = 0.7;
        averageX /= 6;
        averageY /= 6;
        averageZ /= 6;
        if(!(input->x < averageX + 1 && input->x > averageX - 1)) {
                input->x = (averageX * bias) + (1-bias) * input->x;
        }
        if(!(input->y < averageY + 1 && input->y > averageY - 1)) {
                input->y = (averageY * bias) + (1-bias) * input->y;
        }
        if(!(input->z < averageZ + 1 && input->z > averageZ - 1)) {
                input->z = (averageZ * bias) + (1-bias) * input->z;
        }

        for(i = 0; i < 5; i++) {
                SCF->Xdata[i] = SCF->Xdata[i+1];
                SCF->Ydata[i] = SCF->Ydata[i+1];
                SCF->Zdata[i] = SCF->Zdata[i+1];
        }
        SCF->Xdata[5] = input->x;
        SCF->Ydata[5] = input->y;
        SCF->Zdata[5] = input->z;

}

void calculateFusion(kalman_t *kalmanRoll, kalman_t *kalmanPitch){
        int i;
        float dt = calculateTime();

        if(waitOnce) {
                gyroCalculate(dt);
        }
        else{
                currentAngle.x = computeRoll();
                currentAngle.y = computePitch(currentAngle.x);
                currentAngle.x *= 180 /M_PI;
                currentAngle.y *= 180 /M_PI;

                setAngle(kalmanRoll, currentAngle.x);
                setAngle(kalmanPitch, currentAngle.y);

        }

        LM303_accel_read();
        printf("rawx = %f\n", accel_data.x);
        currentAngle.x = computeRoll();
        currentAngle.y = computePitch(currentAngle.x);
        currentAngle.x *= 180 /M_PI;
        currentAngle.y *= 180 /M_PI;

        if ((currentAngle.x < -90 && kalmanRoll->angle > 90) ||
            (currentAngle.x > 90 && kalmanRoll->angle < -90)) {
                setAngle(kalmanRoll, currentAngle.x);
                kalmanRoll->angle = currentAngle.x;
                gyro_data.x = currentAngle.x;
        }
        else {
                getAngle(kalmanRoll, currentAngle.x, gyro_rate.x, dt);
        }

        if (abs(kalmanRoll->angle) > 90) {
                gyro_rate.y = -gyro_rate.y;
        }
        getAngle(kalmanPitch, currentAngle.y, gyro_rate.y, dt);

        if (gyro_data.x < -180 || gyro_data.x > 180) {
                gyro_data.x = kalmanRoll->angle;
        }
        if (gyro_data.y < -180 || gyro_data.y > 180) {
                gyro_data.y = kalmanPitch->angle;
        }
        LM303_mag_read();

        float rollSin = sin(kalmanRoll->angle * M_PI/180);
        float rollCos = cos(kalmanRoll->angle * M_PI/180);
        float pitchSin = sin(kalmanPitch->angle * M_PI/180);
        float pitchCos = cos(kalmanPitch->angle * M_PI/180);

        currentAngle.z = atan2(mag_data.z * rollSin - mag_data.y * rollCos,
                               mag_data.x * pitchCos +
                               mag_data.y * pitchSin * rollSin +
                               mag_data.z * pitchSin * rollCos);
        currentAngle.z *= 180/M_PI;

        waitOnce = 1;
}
