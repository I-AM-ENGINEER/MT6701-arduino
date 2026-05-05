// WARNING: This sketch permanently changes the MT6701 I2C address from
// the default (0x06) to the alternate address (0x46).
// After running, the sensor will always respond at 0x46 on power-up.
// Use example 10_dual_i2c_address_read to read two sensors on the same bus.

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  encoder.initializeI2C();

  Serial.println("Reading angle for 3 seconds...");
  unsigned long start = millis();
  while (millis() - start < 3000) {
    Serial.print("Angle: ");
    Serial.println(encoder.angleRead());
    delay(500);
  }

  Serial.println();
  Serial.println("WARNING: I2C address will be permanently changed to 0x46!");
  for (int i = 10; i > 0; i--) {
    Serial.println(i);
    delay(1000);
  }

  if (encoder.i2cAddressChangeToAlternate()) {
    Serial.println("Success. Sensor now responds at 0x46.");
  } else {
    Serial.println("Address change failed. Sensor still at 0x06.");
  }
}

void loop() {
  Serial.print("Angle: ");
  Serial.println(encoder.angleRead());
  delay(500);
}
