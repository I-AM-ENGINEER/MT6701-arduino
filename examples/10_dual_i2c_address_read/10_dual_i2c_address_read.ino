// Reads two MT6701 sensors on the same I2C bus using different addresses:
//   Sensor 1: default address 0x06
//   Sensor 2: alternate address 0x46
//
// NOTE: One sensor must have its address changed to 0x46 before
// using this example. Use example 09_i2c_address_change for that.

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder1;
MT6701 encoder2;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  encoder1.initializeI2C(&Wire, MT6701_DEFAULT_ADDRESS);
  encoder2.initializeI2C(&Wire, MT6701_ALTERNATE_ADDRESS);
}

void loop() {
  Serial.print("Sensor 1 (0x06): ");
  Serial.print(encoder1.angleRead());
  Serial.print(" deg  |  Sensor 2 (0x46): ");
  Serial.print(encoder2.angleRead());
  Serial.println(" deg");
  delay(100);
}
