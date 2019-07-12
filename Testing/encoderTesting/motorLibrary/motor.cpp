
#include "motor.h"

motor_t motor1, motor2, motor3, motor4;
motor_t motorArray[4] = {motor1, motor2, motor3, motor4};
long looptimer;

void motorInitialize() {

        for (int i = 0; i < 4; i++) {
                motorArray[i].motor.writeMicroseconds(2000);
                delay(2000);
                motorArray[i].motor.writeMicroseconds(1000);
                delay(2000);
                Serial.println("Initialized motor: " + i);
        }

        delay(1000);

}
void motorInitializev2(){

        long runTimer = millis();

        while(millis() - runTimer < 3000) {

                changeSpeed(1000,1000,1000,1000);
        }
        runTimer = millis();
        while(millis() - runTimer < 3000) {
                changeSpeed(2000,2000,2000,2000);
        }
}

void changeSpeed(int m1, int m2, int m3, int m4){

        while(micros() - looptimer < 4000) ;
        looptimer = micros();
        PORTB = B00001000; // pin 11 on to signal no interrupts
        //Serial.println("start");
        PORTD |= B11110000;
        do {
                long esc_loop_timer = micros();
                //Serial.println("a");
                if(looptimer + m1 <= esc_loop_timer) PORTD &= B11101111;
                if(looptimer + m2 <= esc_loop_timer) PORTD &= B11011111;
                if(looptimer + m3 <= esc_loop_timer) PORTD &= B10111111;
                if(looptimer + m4 <= esc_loop_timer) PORTD &= B01111111;
                //Serial.println(PORTD);
        } while (PORTD >= 16);
        //delay(5000);
        //Serial.println("finish");
        PORTB = B00000000; // pin 11 off to enable internets
        //delay(5000);
        //delay(10);
}

void motorUpdate(int m1, int m2, int m3, int m4) {
        motorArray[0].motor.writeMicroseconds(m1+motorArray[0].calibration);
        motorArray[1].motor.writeMicroseconds(m2+motorArray[1].calibration);
        motorArray[2].motor.writeMicroseconds(m3+motorArray[2].calibration);
        motorArray[3].motor.writeMicroseconds(m4+motorArray[3].calibration);
}
