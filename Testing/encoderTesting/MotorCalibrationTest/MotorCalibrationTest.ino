#include <MotorCalibrator.h>

MotorCalibrator calibrator1;

char rx_byte;
void setup() {
  Serial.begin(115200);

//  calibrator1 = new MotorCalibrator();
  
  //id, motor pin, encoder pin
  calibrator1.setup(0, 4, 2);
  
  delay(100);
}

void loop() {
  calibrator1.calibrate();

}
