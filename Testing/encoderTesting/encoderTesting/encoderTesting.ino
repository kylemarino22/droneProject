#include <OpticalEncoder.h>

OpticalEncoder encoder1(2);

void setup() {
  Serial.begin(115200);

}
void loop() {

  encoder1.read();
  Serial.println(encoder1.speed);
}
