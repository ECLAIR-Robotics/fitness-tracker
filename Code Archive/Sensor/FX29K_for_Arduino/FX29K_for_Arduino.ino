#include "FX29K.h"
// #include <Wire.h>

FX29K scale(FX29K0, 0100);

void setup(){
  Wire.begin();
  Serial.begin(9600);
  scale.tare(1000);
}

void loop(){
  Serial.println("Grams\tPounds\tRaw");
  uint16_t raw = scale.getRawBridgeData();
  float g = scale.getGrams();
  float lb = scale.getPounds();
  Serial.print(g, 1);
  Serial.print("\t");
  Serial.print(lb, 3);
  Serial.print("\t");
  // Serial.println(raw);
  delay(1000);
}
