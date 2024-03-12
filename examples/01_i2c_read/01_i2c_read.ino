#include <Wire.h>
#include "MT6701.h"

MT6701 encoder;

void setup() {
	Serial.begin(115200);
	Wire.begin();
    encoder.initializeI2C();
}

void loop() {
	float angle = encoder.angleRead();
	Serial.println(angle);
	delay(100);
}
