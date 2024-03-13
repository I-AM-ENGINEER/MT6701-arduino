/*
  Just upload and use oscilloscope on pin Analog/PWM of MT6701 for see changes
*/

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Wire.begin();
  encoder.initializeI2C();
}


void loop() {
  // Set work angles
  // Combine with offsetSet() for better range selection
  encoder.analogModeSet(90, 270);
  delay(1000);
  // Set full range
  encoder.analogModeSet(0, 360);
  delay(1000);
}
