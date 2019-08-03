
#include "MotorController.h"

MotorController::MotorController() {}

void MotorController::setup(int id, int motorPin){
    this->id = id;

    motor.attach(motorPin);


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

void MotorController::initializeMotor() {
    motor.writeMicroseconds(2000);
    delay(2000);
    motor.writeMicroseconds(1000);
    delay(2000);
    Serial.println("Initialized Motor");
}

void MotorController::readPair(int address) {

}

void MotorController::setSpeed(double speed) {

}