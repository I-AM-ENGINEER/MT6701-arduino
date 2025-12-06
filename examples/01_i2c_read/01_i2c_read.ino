//Pins will be differrent on your target board.
const int I2C_SDA_PIN = 21; const int I2C_SDA_PIN2 = 22;
const int I2C_SCL_PIN = 5; const int I2C_SCL_PIN2 = 3;  

#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;
MT6701 encoder2;

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  encoder.initializeI2C(&Wire);
  // Can use Wire1 too
  Wire1.begin(I2C_SDA_PIN2, I2C_SCL_PIN2);
  encoder2.initializeI2C(&Wire1);
}

void loop() {
  float angle = encoder.angleRead();
  Serial.println("Angle 1");
  Serial.println(angle);
  float angle2 = encoder2.angleRead();
  Serial.println("Angle 2");
  Serial.println(angle2);
  Serial.println("----");
  delay(100);
}
