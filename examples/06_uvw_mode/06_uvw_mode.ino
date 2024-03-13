#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Wire.begin();
  encoder.initializeI2C();

  // Hom many poles should be emulated
  encoder.uvwModeSet(4);
  // Chip could be setting in ABZ mode wtih MODE pin
}


void loop() {
  // Nothing
}
