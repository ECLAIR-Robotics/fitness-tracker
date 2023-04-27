#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
/*
  BLE_Central_Device.ino

  This program uses the ArduinoBLE library to set-up an Arduino Nano 33 BLE Sense 
  as a central device and looks for a specified service and characteristic in a 
  peripheral device. If the specified service and characteristic is found in a 
  peripheral device, the last detected value of the on-board gesture sensor of 
  the Nano 33 BLE
*/


const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
// represents the service number for IMU
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214"; // random string 
// represents the characteristic number for acceleration

#define BLE_UUID_FLOAT_VALUE                "C8F88594-2217-0CA6-8F06-A4270B675D69" //TODO: Find out more about this
// value to pass or receive floats
#define BLE_UUID_AMPLITUDE                  "E3ADBF53-950E-DC1D-9B44-076BE52760D6"


// int gesture = -1;
// int oldGestureValue = -1;   

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  Serial.println("Initializing Accelerometer");
  Serial.println("X_a,Y_a,Z_a");
  
  BLE.setLocalName("Nano 33 BLE"); 
  BLE.advertise();

  Serial.println("Arduino Nano 33 BLE (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();
}

void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic accelerationCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
    
  if (!accelerationCharacteristic) {
    Serial.println("* Peripheral device does not have acceleration characteristic!");
    peripheral.disconnect();
    return;
  } else if (!accelerationCharacteristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable acceleration characteristic!");
    peripheral.disconnect();
    return;
  }
  float acc_list[3];
  while (peripheral.connected()) {
    retrieveAcceleration(acc_list); // list is in order x, y, z
    Serial.print("* Writing value to acceleration characteristic: ");
    // Serial.println(acc_list);
    accelerationCharacteristic.writeValue((byte)acc_list[0]);
    accelerationCharacteristic.writeValue((byte)acc_list[1]);
    accelerationCharacteristic.writeValue((byte)acc_list[2]);
    Serial.println("* Writing value to acceleration characteristic done!");
    Serial.println(" ");
    delay(500);
  
    // if (oldGestureValue != gesture) {  
    //   oldGestureValue = gesture;
    //   Serial.print("* Writing value to gesture_type characteristic: ");
    //   Serial.println(gesture);
    //   gestureCharacteristic.writeValue((byte)gesture);
    //   Serial.println("* Writing value to gesture_type characteristic done!");
    //   Serial.println(" ");
    // }
  
  }
  Serial.println("- Peripheral device disconnected!");
}

void retrieveAcceleration(float* vals){
  // float x_a, y_a, z_a;
  // float myAxes[3];
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(vals[0], vals[1], vals[2]);
    // Serial.print(x_a);
    // Serial.print(',');
    // Serial.print(y_a);
    // Serial.print(',');
    // Serial.print(z_a);
    // Serial.print(',');
  }else{
    vals[0] = 0;
    vals[1] = 0;
    vals[2] = 0;
  }
  // return myAxes;
}
  
// int gestureDetectection() {
//   if (APDS.gestureAvailable()) {
//     gesture = APDS.readGesture();

//     switch (gesture) {
//       case GESTURE_UP:
//         Serial.println("- UP gesture detected");
//         break;
//       case GESTURE_DOWN:
//         Serial.println("- DOWN gesture detected");
//         break;
//       case GESTURE_LEFT:
//         Serial.println("- LEFT gesture detected");
//         break;
//       case GESTURE_RIGHT:
//         Serial.println("- RIGHT gesture detected");
//         break;
//       default:
//         Serial.println("- No gesture detected");
//         break;
//       }
//     }
//     return gesture;
// }