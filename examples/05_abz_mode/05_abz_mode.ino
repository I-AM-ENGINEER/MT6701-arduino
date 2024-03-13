#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Wire.begin();
  encoder.initializeI2C();
  
  // First argument - how many steps per rotation, 1...1024
  // Second - pulse width (read datasheet), can be:
  //    MT6701_PULSE_WIDTH_1LSB
  //    MT6701_PULSE_WIDTH_2LSB
  //    MT6701_PULSE_WIDTH_4LSB
  //    MT6701_PULSE_WIDTH_8LSB
  //    MT6701_PULSE_WIDTH_12LSB
  //    MT6701_PULSE_WIDTH_16LSB
  //    MT6701_PULSE_WIDTH_180
  // Third - hysteresis, can be:
  //    MT6701_HYST_0_25
  //    MT6701_HYST_0_5
  //    MT6701_HYST_1
  //    MT6701_HYST_2
  //    MT6701_HYST_4
  //    MT6701_HYST_8
  encoder.abzModeSet(32, MT6701_PULSE_WIDTH_180, MT6701_HYST_1);
  // Chip could be setting in ABZ mode wtih MODE pin
}


void loop() {
  // Nothing
}
