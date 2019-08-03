#include <OpticalEncoder.h>

#include <motor.h>

OpticalEncoder encoder1;

char rx_byte;

template <typename Proc> void do_something(Proc p)
{
  p();
}

void setup() {
  Serial.begin(115200);

  motor1.motor.attach(5);


  auto b= []() { Serial.println("a"); };
  
  encoder1.setup(b);

//  do_something(b);

  
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

}
