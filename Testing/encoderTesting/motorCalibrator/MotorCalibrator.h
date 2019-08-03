
#ifndef _MotorCalibrator_h_
#define _MotorCalibrator_h_

#include <EEPROM.h>
#include <OpticalEncoder.h>
#include <Servo.h>
#include "Arduino.h"

#define CALIBRATION_MAX_DATA 50
#define PAIR_SIZE 6

typedef struct pair {
        uint8_t key;
        float value;
} pair_t;


class MotorCalibrator {
private:
    int id;
    int currentEEPROMAdress = 0;
    OpticalEncoder * encoder;
    Servo motor;
    void writeEEPROM (pair_t data);
    void clearEEPROM ();
    void initializeMotor ();
public:
    MotorCalibrator ();
    void setup (int id, int motorPin, int encoderPin);
    void calibrate ();
};

#endif
