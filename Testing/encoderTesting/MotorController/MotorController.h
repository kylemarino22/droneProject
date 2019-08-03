
#ifndef _MotorController_h_
#define _MotorController_h_

#include <EEPROM.h>
#include <Servo.h>
#include "Arduino.h"

#define PAIR_SIZE 6

typedef struct pair {
        uint8_t key;
        float value;
} pair_t;


class MotorController {
private:
    int id;
    int currentEEPROMAdress = 0;
    Servo motor;
    void:
        initializeMotor ();
        readPair(int address);
public:
    MotorController ();
    void:
        setup (int id, int motorPin);
        setSpeed (double speed);
};

#endif
