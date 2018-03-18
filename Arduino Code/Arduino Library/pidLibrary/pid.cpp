#include "pid.h"
#include "sensors.h"
#include <Arduino.h>

pid rollPID;
pid pitchPID;
pid yawPID;

float calculatePID(pid *pid, float angle, float input) {
        float error = input - angle;

        Serial.print(error);
        if(abs(error) < 3)
        {
                pid->i += error * pid->iGain;
                Serial.print("\t"); Serial.println("less");

        }
        else{
                Serial.println();
        }
        float p = error * pid->pGain;
        float d1;
        float d2;
        // if(abs(error)>2) {
        d1 = ((error - pid->lastError) / 0.02);
        d2 = d1*pid->dGain;
        pid->lastError = error;

        // float stableAverage;
        //
        // for(int i = 0; i < 5; i++) {
        //         stableAverage += pid->stabilized[i];
        // }
        // stableAverage /= 5;
        // int flag = 1;
        // for(int i = 0; i < 5; i++) {
        //
        //         if(!(pid->stabilized[i] < stableAverage +1 && pid->stabilized[i] > stableAverage -1)) {
        //                 flag = 0;
        //         }
        // }

        // if(flag == 1) {
        //         Serial.println("stable");
        //         if(stableAverage > angle + 3) {
        //                 pid->offSet -=0.01;
        //                 Serial.println("left");
        //
        //         }
        //         if(stableAverage < angle - 3) {
        //                 pid->offSet += 0.01;
        //                 Serial.println("right");
        //         }
        // }


        //Serial.println(error-pid->lastError);
        pid->lastD = d1;

        //
        //
        // if(pid->counter %100 == 0) {
        //         pid->stabilized[pid->counter/100] = error;
        // }
        //
        // pid->counter++;
        // if(pid->counter > 500) {
        //         pid->counter = 0;
        // }

        return (d2 + p + pid->i - pid->offSet);
}
