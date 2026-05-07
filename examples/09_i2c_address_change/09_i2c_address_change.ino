// This sketch changes the MT6701 I2C address between default (0x06) and
// alternate (0x46) and optionally saves the result to EEPROM.
// Use example 10_dual_i2c_address_read to read two sensors on the same bus.

// Set to 1 to change from default (0x06) -> alternate (0x46)
// Set to 0 to change from alternate (0x46) -> default (0x06)
#define CHANGE_TO_ALTERNATE 1

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Serial.begin(115200);
  Wire.begin();
#if CHANGE_TO_ALTERNATE
  encoder.initializeI2C(&Wire, MT6701_DEFAULT_ADDRESS);
#else
  encoder.initializeI2C(&Wire, MT6701_ALTERNATE_ADDRESS);
#endif

  Serial.println("Reading angle for 3 seconds...");
  unsigned long start = millis();
  while (millis() - start < 3000) {
    Serial.print("Angle: ");
    Serial.println(encoder.angleRead());
    delay(100);
  }

  Serial.println();

#if CHANGE_TO_ALTERNATE
  Serial.println("Changing address: 0x06 -> 0x46...");
  bool ok = encoder.i2cAddressChangeToAlternate();
#else
  Serial.println("Changing address: 0x46 -> 0x06...");
  bool ok = encoder.i2cAddressChangeToDefault();
#endif

  if (ok) {
    Serial.println("Success.");
  } else {
    Serial.println("Failed.");
  }
}

void loop() {
  Serial.print("Angle: ");
  Serial.println(encoder.angleRead());
  delay(100);
}
