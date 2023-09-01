/*

*/

#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("Initializing Accelerometer, Magnetometer, and Gyroscope");
  Serial.println("X_a,Y_a,Z_a,X_m,Y_m,Z_m,X_g,Y_g,Z_g");
}

void loop() {
  float x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_a, y_a, z_a);

    Serial.print(x_a);
    Serial.print(',');
    Serial.print(y_a);
    Serial.print(',');
    Serial.print(z_a);
    Serial.print(',');
  }else{
    Serial.print(",,,");
  }
  
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x_m, y_m, z_m);

    Serial.print(x_m);
    Serial.print(',');
    Serial.print(y_m);
    Serial.print(',');
    Serial.print(z_m);
    Serial.print(',');
  }else{
    Serial.print(",,,");
  }


  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x_g, y_g, z_g);

    Serial.print(x_g);
    Serial.print(',');
    Serial.print(y_g);
    Serial.print(',');
    Serial.println(z_g);
  }else{
    Serial.println(",,");
  }
  
  delay(500);
}