#include <OpticalEncoder.h>

#include <motor.h>

OpticalEncoder * encoder1;

char rx_byte;
void setup() {
  Serial.begin(115200);

  motor1.motor.attach(5);

  encoder1 = new OpticalEncoder();
  encoder1->setup(2, []{encoder1->read();}, CHANGE);
  
  Serial.println("Initialize motors? (y/n)");

  while(1) {
    //waiting for serial response
    if (Serial.available() > 0) {
      rx_byte = Serial.read();
    
      if (rx_byte == 'y') {
        Serial.println("Initializing motors");
        motorInitialize();
        break;
      }
      else {
        Serial.println("Not initializing motors");
        break;
      }
    }
  }
  
  motorUpdate(1200, 1200, 1200, 1200);
  delay(100);
}

double rps = 0;
double counter = 0;
int motorIncrement = 0;
void loop() {
  if (encoder1->processBuffer()) {
    rps = ((rps * counter) + encoder1->speed) / ++counter;
    
    if ((int)counter % 100 == 0) {
      Serial.print("Motor Speed: ");
      Serial.print(1200 + motorIncrement * 50);
      Serial.println(rps);
      motorIncrement++;
      motorUpdate(1200 + motorIncrement * 50, 1000, 1000, 1000);
    }
  }
}
