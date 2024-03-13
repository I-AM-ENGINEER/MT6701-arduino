/*
	Just upload and use oscilloscope/frequency meter on pin Analog/PWM of MT6701 for see changes
*/

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Wire.begin();
  encoder.initializeI2C();
}


void loop() {
  // PWM frequncy could be only:
  //    MT6701_PWM_FREQ_497_2
  //    MT6701_PWM_FREQ_994_4
  // Polaty could be:
  //    MT6701_PWM_POL_HIGH
  //    MT6701_PWM_POL_LOW

  encoder.pwmModeSet(MT6701_PWM_FREQ_497_2, MT6701_PWM_POL_HIGH);
  delay(1000);
  encoder.pwmModeSet(MT6701_PWM_FREQ_994_4, MT6701_PWM_POL_LOW);
  delay(1000);
}
