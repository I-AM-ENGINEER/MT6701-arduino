#include <Arduino.h>
#include <SPI.h>
#include "MT6701.h"

#define CS_PIN  9

MT6701 encoder;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  encoder.initializeSSI(CS_PIN);
  Serial.println("Init complete");
}

void loop() {
  mt6701_status_t field_status = encoder.fieldStatusRead();
  float angle = encoder.angleRead();

  Serial.print("field: ");
  switch (field_status){
    case MT6701_STATUS_NORM: Serial.print("NORM"); break;
    case MT6701_STATUS_FIELD_STRONG: Serial.print("STRONG"); break;
    case MT6701_STATUS_FIELD_WEAK: Serial.print("WEAK"); break;
    default: Serial.print("ERROR"); break;
  }
  Serial.print("   |    angle: ");
  Serial.println(angle);

  delay(100);
}
