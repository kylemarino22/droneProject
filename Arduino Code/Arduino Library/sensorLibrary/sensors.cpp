#include "sensors.h"

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(54321);
Adafruit_L3GD20 gyro = Adafruit_L3GD20();

XYZ kalAngle;
XYZ gyroAngle;
XYZ gyro_calibrate;
XYZ accel_calibrate;
XYZ accelXYZ;
XYZ magXYZ;
XYZ gyroXYZ;
XYZ gyroRate;
XYZ current;

Kalman kalmanX;
Kalman kalmanY;

bool waitOnce = false;

long long currentRead, lastRead;
float angle_roll;
float angle_pitch;

void calibrateGyro() {
        float tempX = 0;
        float tempY = 0;
        float tempZ = 0;
        for (int i = 0; i < 500; i++) {
                gyro.read();
                tempX += gyro.data.x;
                tempY += gyro.data.y;
                tempZ += gyro.data.z;
                delay(3);
        }
        gyro_calibrate = {tempX / 500, tempY / 500, tempZ / 500};
}

void calibrateAccel() {
        float tempX = 0;
        float tempY = 0;
        float tempZ = 0;
        sensors_event_t event;
        for (int i = 0; i < 500; i++) {
                accel.getEvent(&event);
                tempX += event.acceleration.x;
                tempY += event.acceleration.y;
                tempZ += event.acceleration.z - 9.81;
                delay(3);
        }
        accel_calibrate = {tempX / 500, tempY / 500, tempZ / 500};
}

void beginSensors(boolean calibrate) {

        if (!accel.begin())
        {
                Serial.println("I2C failed");
                while (1) ;
        }
        mag.begin();
        gyro.begin(GYRO_RANGE_500DPS);
        delay(500);
        if (calibrate == true) {
                calibrateGyro();
                calibrateAccel();
        }
        Serial.println(accel_calibrate.x);
        Serial.println(accel_calibrate.y);
        Serial.println(accel_calibrate.z);
        Serial.println("============");

}

void computeRoll() {
        current.x = atan2(accelXYZ.y, accelXYZ.z);
}

void computePitch() {
        float rollSin = sin(current.x);
        float rollCos = cos(current.x);
        current.y = atan2(-accelXYZ.x, accelXYZ.y * rollSin + accelXYZ.z * rollCos);
}
float testX;
float testY;
float testZ;
void calculateGyro(float dt) {

        testX = (gyro.data.x - gyro_calibrate.x)  * 0.0175;// * 0.5 + testX * 0.5;
        testY = (gyro.data.y - gyro_calibrate.y)  * 0.0175;// * 0.5 + testY * 0.5;
        testZ = (gyro.data.z - gyro_calibrate.z)  * 0.0175;// * 0.5 + testZ * 0.5;

        //Serial.println(testX);

        if (abs(testZ) > 0.5) {
                gyroXYZ.z += testZ * (dt);
        }
        else {
                testZ = 0;
        }

        if (abs(testY) > 0.5) {
                gyroXYZ.y += testY * (dt);
        }
        else {
                testY = 0;
        }

        if (abs(testX) > 0.5) {
                gyroXYZ.x += testX * dt;
        }
        else {
                testX = 0;
        }

        gyroXYZ.x += gyroXYZ.y * sin(testZ * dt * (3.1415 / 180));
        gyroXYZ.y -= gyroXYZ.x * sin(testZ * dt * (3.1415 / 180));

        gyroRate = {testX, testY, testZ};
}

float calculateTime() {

        currentRead = micros();
        float dt = currentRead - lastRead;
        lastRead = currentRead;
        return dt/1000000;
}

void calculateFusion2(){
        float dt = 0.004;
        sensors_event_t event;
        gyro.read();
        calculateGyro(dt);
        //Serial.println("pass");
        accel.getEvent(&event);
        accelXYZ = XYZ {(event.acceleration.x - accel_calibrate.x),
                        (event.acceleration.y - accel_calibrate.y),
                        (event.acceleration.z - accel_calibrate.z)};
        computeRoll();
        computePitch();
        current.x *=  180 / PI;
        current.y *= 180 / PI;

        //Serial.println(gyroXYZ.y);
        // //angle_roll = gyroXYZ.y;
        // Serial.print(gyroXYZ.y); Serial.print("\t");
        // Serial.print(current.y); Serial.print("\t");
        // angle_pitch = gyroXYZ.y;
        //
        gyroXYZ.y = gyroXYZ.y * 0.995 + current.y * 0.005;
        gyroXYZ.x = gyroXYZ.x * 0.995 + current.x * 0.005;
        // angle_pitch = angle_pitch * 0.9996 + current.x * 0.0004;
        //
        // Serial.println(gyroXYZ.y);


}

void calculateFusion() {
        float dt = calculateTime();
        sensors_event_t event;
        gyro.read();

        if (waitOnce) {
                calculateGyro(dt);
        }
        else {
                accel.getEvent(&event);
                accelXYZ = XYZ {event.acceleration.x, event.acceleration.y, event.acceleration.z};

                computeRoll();
                computePitch();
                current.x *=  180 / PI;
                current.y *= 180 / PI;

                kalmanX.setAngle(current.x);
                kalmanY.setAngle(current.y);
                gyroXYZ.x = current.x;
                gyroXYZ.y = current.y;
        }

        accel.getEvent(&event);
        accelXYZ = XYZ {event.acceleration.x, event.acceleration.y, event.acceleration.z};

        computeRoll();
        computePitch();
        current.x *= 180 / PI;
        current.y *= 180 / PI;

        if ((current.x < -90 && kalAngle.x > 90) || (current.x > 90 && kalAngle.x < -90)) {
                kalmanX.setAngle(current.x);
                kalAngle.x = current.x;
                gyroXYZ.x = current.x;
        }
        else {

                kalAngle.x = kalmanX.getAngle(current.x, gyroRate.x, dt);

        }

        if (abs(kalAngle.x) > 90) {
                gyroRate.y = -gyroRate.y;
        }

        kalAngle.y = kalmanY.getAngle(current.y, gyroRate.y, dt);

        if (gyroXYZ.x < -180 || gyroXYZ.x > 180) {
                gyroXYZ.x = kalAngle.x;
        }
        if (gyroXYZ.y < -180 || gyroXYZ.y > 180) {
                gyroXYZ.y = kalAngle.y;
        }

        mag.getEvent(&event);
        magXYZ = XYZ {event.magnetic.x, event.magnetic.y, event.magnetic.z};

        float rollSin = sin(kalAngle.x * PI / 180);
        float rollCos = cos(kalAngle.x * PI / 180);
        float pitchSin = sin(kalAngle.y * PI / 180);
        float pitchCos = cos(kalAngle.y * PI / 180);

        current.z = atan2(magXYZ.z * rollSin - magXYZ.y * rollCos,
                          magXYZ.x * pitchCos +
                          magXYZ.y * pitchSin * rollSin +
                          magXYZ.z * pitchSin * rollCos);

        current.z *= 180 / PI;
        waitOnce = true;
}
