
#include "MotorCalibrator.h"

MotorCalibrator::MotorCalibrator() {}

void MotorCalibrator::setup(int id, int motorPin, int encoderPin){
    this->id = id;
//    clearEEPROM();

    motor.attach(motorPin);

    encoder = new OpticalEncoder();
    encoder->setup(encoderPin);

    char rx_byte;
    Serial.println("Initialize Motors?");

    while(1) {
        //waiting for serial response
        if (Serial.available() > 0) {
            rx_byte = Serial.read();

            if (rx_byte == 'y') {
                Serial.println("Initializing motors");
                initializeMotor();
                break;
            }
            else {
                Serial.println("Not initializing motors");
                break;
            }
        }
    }

    motor.writeMicroseconds(1000);
    delay(100);
}

void MotorCalibrator::clearEEPROM() {
    for (int i = id * CALIBRATION_MAX_DATA; i < CALIBRATION_MAX_DATA * (id + 1); i++) {
        if (EEPROM.read(i * PAIR_SIZE) != 0) {
            for (int j = 0; j < PAIR_SIZE; j++) {
                EEPROM.write(i * PAIR_SIZE + j, 0);
            }
        }
    }
    Serial.println("EEPROM erased");
}

void MotorCalibrator::initializeMotor() {
    motor.writeMicroseconds(2000);
    delay(2000);
    motor.writeMicroseconds(1000);
    delay(2000);
    Serial.println("Initialized Motor");
}

void MotorCalibrator::writeEEPROM(pair_t data){
    int address = id * CALIBRATION_MAX_DATA + currentEEPROMAdress * PAIR_SIZE;
    EEPROM.put(address, data);
}

void MotorCalibrator::calibrate() {

    double rps = 0;
    double counter = 0;
    int motorIncrement = 0;
    pair_t writePair;
    encoder->clear();

    while(1) {
        if (encoder->processBuffer()) {
            Serial.println("a");

            //avg rps calculation over 100 reads
            rps = ((rps * counter) + encoder->speed) / ++counter;

            //100 reads per calibration
            if ((int)counter > 99) {

                noInterrupts();
                Serial.print("Motor Input: ");
                Serial.print(1000 + motorIncrement * 50);
                Serial.print("   Motor Speed: ");
                Serial.println(rps);
                Serial.println("=========================");

                //write data to EEPROM
                writePair.key = motorIncrement;
                writePair.value = (float) rps;
                writeEEPROM(writePair);

                if (motorIncrement == 20) {
                    //Turn off encoder and motor
                    noInterrupts();
                    motor.writeMicroseconds(1000);
                    Serial.println("Calibration Finished!");
                    break;
                }

                //reset loop variables
                counter = 0;
                rps = 0;
                motorIncrement++;

                //write to motor and wait for speed change
                interrupts();
                motor.writeMicroseconds(1000 + motorIncrement * 50);
                delay(50);

                //clear encoder
                encoder->clear();
            }
        }
    }
}