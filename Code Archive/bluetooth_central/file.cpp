while (central.connected()) {

      // initialize all the variables
      float fx_a, fy_a, fz_a, fx_m, fy_m, fz_m, fx_g, fy_g, fz_g;
      byte x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g;
      x_a = 0;
      y_a = 0;
      z_a = 0;
      x_m = 0;
      y_m = 0;
      z_m = 0;
      x_g = 0;
      y_g = 0;
      z_g = 0;

      // poll acc
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(fx_a, fy_a, fz_a);
        x_a = fx_a * 100;
        y_a = fy_a * 100;
        z_a = fz_a * 100;
        Serial.print(x_a);
        Serial.print(',');
        Serial.print(y_a);
        Serial.print(',');
        Serial.print(z_a);
        Serial.print(';');
      }else{
        Serial.print("0,0,0;");
      }
      if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(fx_m, fy_m, fz_m);
        x_m = fx_m * 100;
        y_m = fy_m * 100;
        z_m = fz_m * 100;
        Serial.print(x_m);
        Serial.print(',');
        Serial.print(y_m);
        Serial.print(',');
        Serial.print(z_m);
        Serial.print(';');
      }else{
        Serial.print("0,0,0;");
      }

      if (IMU.accelerationAvailable()) {
        IMU.readGyroscope(fx_g, fy_g, fz_g);
        x_g = fx_g * 100;
        y_g = fy_g * 100;
        z_g = fz_g * 100;
        Serial.print(x_g);
        Serial.print(',');
        Serial.print(y_g);
        Serial.print(',');
        Serial.println(z_g);
      }else{
        Serial.print("0,0,0;");
      }

      byte  arr [] = {x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g};
      // write to characteristic 
      accCharacteristic.writeValue(arr, 9);
    }