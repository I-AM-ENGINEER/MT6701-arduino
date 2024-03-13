#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Wire.begin();
    encoder.initializeI2C();
  
  // Configure encoder settings

  // Direction could be:
  // MT6701_DIRECTION_CW
  // MT6701_DIRECTION_CCW
  encoder.directionSet(MT6701_DIRECTION_CW);
  encoder.offsetSet(45.0f);
  encoder.pwmModeSet(MT6701_PWM_FREQ_994_4, MT6701_PWM_POL_LOW);
  // Save settings in chip EEPROM
  encoder.programmEEPROM();
}


void loop() {
  // Nothing
}
