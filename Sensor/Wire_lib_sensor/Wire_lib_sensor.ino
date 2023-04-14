#include <Wire.h>

#define UseDebug 1

#define clockFrequency 400000
#define zeroOffset 940            // fine-tuned for a specif load cell, should stay at 1000 normally

float scaleFactor = 1400;         // 14000 counts / 10 pounds
int curAddress = 0x28;

void setup() {
  Wire.begin(); //Starts I2C, will need to change these two values (or delete them)
  Wire.setClock(clockFrequency); //Sets I2C frequency to fastest
  Serial.begin(9600);
}

void loop() {
  Wire.requestFrom(curAddress, 0, true); 
  // Wire.beginTransmission(curAddress); //Request Measurement
  // Wire.endTransmission();
  
  delay(100); //Delay for measurement to be taken (could be done in IRQ)
  
  Wire.requestFrom(curAddress, 2, true); //Requests two bytes of data
  if (!Wire.available()){
    return;
  }
  byte msb = Wire.read();
  byte lsb = Wire.read();
  
  #if (UseDebug)
  byte statusVals = msb/64; //Determines the status values from the measurement to ensure a new measurement was taken
  Serial.print("status:");
  Serial.println(statusVals, BIN);
  Serial.print("msb:");
  Serial.println(msb, BIN);
  Serial.print("lsb:");
  Serial.println(lsb, BIN);
  #endif
  
  int dataVal = (msb%64)*256 + lsb; //Throws out the status bits and converts the measurement data into a single int
  Serial.print("dataValue:");
  Serial.println(dataVal);
  
  float weightVal = ((dataVal-zeroOffset)/scaleFactor);
  
  Serial.println("Weight: " + String(weightVal) + " lbs");
  
  Serial.println();
  delay(1000);
}