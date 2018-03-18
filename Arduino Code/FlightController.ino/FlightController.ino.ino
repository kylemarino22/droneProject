#include <Serial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Kalman.h>
#include <Servo.h>
#include <motor.h>
#include <pid.h>
#include <sensors.h>
#include <tachometer.h>




bool CALIBRATE = true;

bool flag = true;

int initialize = 0;
int throttleSetpoint;
int pitchSetpoint;
int rollSetpoint;
int yawSetpoint;
//tach_t pin1;
//tach_t pin0;

void setup(void)
{
  Serial.begin(115200);
  //  beginSerial();
  pinMode(13,OUTPUT);
  while (1) {
    digitalWrite(13,HIGH);
    delay(1000);
    
    requestSerial();
    if (commandRecieved == 5 && dataRecieved == 1) {
      initialize = 0;
      break;
    }
    if (commandRecieved == 5 && dataRecieved == 2) {
      initialize = 1;
      break;
    }
    Serial.write(20);
  }
  pinMode(13,LOW);
  //  rollPID.pGain = 1.75; // 0.8
  //rollPID.iGain = 0.0001; // 0.1
  //rollPID.dGain = 0.73; // 4
  pin1.pin = 1;
  pin0.pin = 0;


  motor1.motor.attach(5);
  motor2.motor.attach(6);

  if (initialize == 1) {
    motorInitialize();
  }
  motorUpdate(1000, 1000, 1000, 1000);

  beginSensors(CALIBRATE);
  //calibrateTachometer(&pin1);

}

bool runOnce = true;
int counter = 0;
unsigned long loop_timer;
void loop(void)
{

  //    if (runOnce) {
  //      syncMotors();
  //      runOnce = false;
  //    }
  requestSerial();
  if (commandRecieved == 1) {
    throttleSetpoint = (dataRecieved * 5)+1000;
  }
  if (commandRecieved == 2) {
    rollSetpoint = dataRecieved - 90;
  }
  if (commandRecieved == 3) {
    pitchSetpoint = dataRecieved - 90;
  }
  if (commandRecieved == 4) {
    yawSetpoint = dataRecieved;
  }

  if(throttleSetpoint == 1200){
    Serial.write(21);
  }

  Serial.write(91);
  calculateFusion2();

  if (counter > 5) {
    float correction = calculatePID(&rollPID, 0, gyroXYZ.y);
    //Serial.println(correction);

    motorUpdate(1100 - correction, 1100 + correction, 1000, 1000);
    counter = 0;
  }

  if (counter == 3) {

  }

  //  if(micros - loop_timer > 4500){
  //    Serial.println("loop too long");
  //  }
  if(micros()-loop_timer > 4500){
    Serial.write(56);
  }
  while (micros() - loop_timer < 4000);
  loop_timer = micros();


  Serial.write(43);
  Serial.write(commandRecieved);
  Serial.write(dataRecieved);

  if (commandRecieved == 5 && dataRecieved == 3) {
    // read the incoming byte:
      Serial.write(12);
      motor1.motor.writeMicroseconds(1000);
      motor2.motor.writeMicroseconds(1000);
      while (1);

  }
  counter ++;
}

