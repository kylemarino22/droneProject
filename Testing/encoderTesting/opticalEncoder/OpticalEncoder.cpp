
#include "opticalEncoder.h"

OpticalEncoder::OpticalEncoder () {}

OpticalEncoder::OpticalEncoder (int pin) {
    pinMode(pin, INPUT);
}

void OpticalEncoder::read () {
    if (digitalRead(2)) {

        if (!pin1_ON) {
            pin1_Switch = true;
        }
        else {
            pin1_Switch = false;
        }
        pin1_ON = true;
    }
    else {

        if (pin1_ON) {
            pin1_Switch = true;
        }
        else {
            pin1_Switch = false;
        }
        pin1_ON = false;
    }


    if (pin1_Switch) {
        delta_t = micros() - prev_time;
        prev_time = micros();

        //current encoder speed
        circularBuffer[pos] = 0.125 / ((double) delta_t / 1000000);
        pos++;
        if (pos > bufferLength) {
            pos = 0;
            maxValue = -10;
            minValue = 99999999;

            for (int i = 0; i < bufferLength; i++){
                if (circularBuffer[i] > maxValue) {
                    maxValue = circularBuffer[i];
                    maxValuePos = i;
                }

                if (circularBuffer[i] < minValue) {
                    minValue = circularBuffer[i];
                    minValuePos = i;
                }
            }

            speed = 0;
            for (int i = 0; i < bufferLength; i++) {
                if (i == maxValuePos || i == minValuePos) continue;
                speed += circularBuffer[i];
            }
            speed /= bufferLength - 2;


        }

    }

}