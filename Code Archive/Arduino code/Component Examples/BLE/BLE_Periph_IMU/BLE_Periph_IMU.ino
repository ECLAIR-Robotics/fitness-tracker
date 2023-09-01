#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// BLEService ledService("180A"); // BLE LED Service

BLEService imuService("180A"); // BLE IMU Service

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
// represents the service number for IMU
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214"; // random string 
// represents the characteristic number for acceleration


// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
// BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);

// BLE Acc Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic accCharacteristic("2A57", BLERead, 18);
BLEByteCharacteristic timeCharacteristic("8AD3", BLERead);
// BLECharacteristic controlCharacteristic("BC76", BLERead, 4);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED's pin to output mode
  // pinMode(LEDR, OUTPUT);
  // pinMode(LEDG, OUTPUT);
  // pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  // digitalWrite(LEDR, HIGH);               // will turn the LED off
  // digitalWrite(LEDG, HIGH);               // will turn the LED off
  // digitalWrite(LEDB, HIGH);                // will turn the LED off

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("Initializing Accelerometer");

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(imuService);
  
  Serial.println("Initializing Accelerometer");

  // add the characteristic to the service
  imuService.addCharacteristic(accCharacteristic);

  // add service
  BLE.addService(imuService);

  // set the initial value for the characteristic:
  accCharacteristic.writeValue((byte)0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE IMU Peripheral");
}


void add_shorts_to_byte_array(short* arr, byte* byte_arr){ 
    // uint32_t val = 0x12345678;   // Value to be written 
    // uint8_t array[24];           // array to which val is to be written
    // uint8_t ptr = &array[14];    // pointer to first byte to be written
    // for (int i=0; i<3; i++)
    // {
    //     ptr[i] = (uint8_t)(val >> (i * 8));  // get one byte of val, convert to unsigned byte, and write to array
    // }
  for (int i = 0; i < 9; ++i){
    short val = arr[i];
    int j = i * 2;
    byte_arr[j] = (byte) (val >> 8);
    byte_arr[j+1] = (byte) val;
  }
}

void print_arr(short* arr) {
  for(int i = 0; i < 9; i++)
  {
    Serial.print(arr[i]);
  }
  Serial.println(";");
}

void print_arr(byte* arr) {
  for(int i = 0; i < 18; i++)
  {
    Serial.print(arr[i], HEX);
  }
  Serial.println(";");
}


void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);            // turn on the LED to indicate the connection

    // while the central is still connected to peripheral:
    while (central.connected()) {

      // initialize all the variables
      float fx_a, fy_a, fz_a, fx_m, fy_m, fz_m, fx_g, fy_g, fz_g;
      short x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g;
      x_a = 0;
      y_a = 0;
      z_a = 0;
      x_m = 0;
      y_m = 0;
      z_m = 0;
      x_g = 0;
      y_g = 0;
      z_g = 0;
      byte byte_arr [18];
      // poll acc
      if (IMU.accelerationAvailable()) { // values from -128 to 127
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
      if (IMU.magneticFieldAvailable()) { // values from -180 to 180 ????
        IMU.readMagneticField(fx_m, fy_m, fz_m);
        x_m = (short) floorf(fx_m);
        y_m = (short) floorf(fy_m);
        z_m = (short) floorf(fz_m);
        Serial.print(x_m);
        Serial.print(',');
        Serial.print(y_m);
        Serial.print(',');
        Serial.print(z_m);
        Serial.print(';');
      }else{
        Serial.print("0,0,0;");
      }

      if (IMU.gyroscopeAvailable()) { // values from -128 to 127 
        IMU.readGyroscope(fx_g, fy_g, fz_g);
        x_g = (short) floorf(fx_g);
        y_g = (short) floorf(fy_g);
        z_g = (short) floorf(fz_g);
        Serial.print(x_g);
        Serial.print(',');
        Serial.print(y_g);
        Serial.print(',');
        Serial.println(z_g);
      }else{
        Serial.println("0,0,0;");
      }
      short  arr [] = {x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g};
      // print_arr(arr);
      add_shorts_to_byte_array(arr, byte_arr);
      // print_arr(byte_arr);
      // write to characteristic 
      accCharacteristic.writeValue(byte_arr, 18);
      delay(100);
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
    // digitalWrite(LEDR, HIGH);          // will turn the LED off
    // digitalWrite(LEDG, HIGH);        // will turn the LED off
    // digitalWrite(LEDB, HIGH);         // will turn the LED off
  }
}