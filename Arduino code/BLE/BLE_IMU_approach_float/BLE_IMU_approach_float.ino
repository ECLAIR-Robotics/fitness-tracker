/*
  This example creates a BLE central that scans for a peripheral with a Test Service
  If that contains floatValue characteristics the value can be seen in the Serial Monitor or Plotter.

  The circuit:
  - Arduino Nano 33 BLE or Arduino Nano 33 IoT board.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------

#define BLE_UUID_TEST_SERVICE               "9A48ECBA-2E92-082F-C079-9E75AAE428B1"
#define BLE_UUID_FLOAT_VALUE                "C8F88594-2217-0CA6-8F06-A4270B675D69"

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  Serial.println("Initialized Accelerometer");
  Serial.println("X_a,Y_a,Z_a");
  
  BLE.setLocalName("Nano 33 BLE"); 
  // BLE.advertise();

  BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
}


void loop()
{
  BLEDevice peripheral = BLE.available();

  if ( peripheral )
  {
    if ( peripheral.localName() != "Arduino Nano 33 BLE" )
    {
      return;
    }

    BLE.stopScan();

    connectPeripheral( peripheral );

    BLE.scanForUuid( BLE_UUID_TEST_SERVICE );
  }
}


bool connectPeripheral( BLEDevice peripheral )
{
  if ( !peripheral.connect() )
  {
    return false;
  }

  if ( !peripheral.discoverAttributes() )
  {
    peripheral.disconnect();
    return false;
  }

  BLECharacteristic floatValueCharacteristic = peripheral.characteristic( BLE_UUID_FLOAT_VALUE );\
  if ( !floatValueCharacteristic )
  {
    peripheral.disconnect();
    return false;
  }

  if ( !floatValueCharacteristic.canSubscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  if ( !floatValueCharacteristic.subscribe() )
  {
    peripheral.disconnect();
    return false;
  }

  while ( peripheral.connected() )
  {
    if ( floatValueCharacteristic.valueUpdated() )
    {
      float floatValue;
      floatValueCharacteristic.readValue( &floatValue, 4 );
      Serial.println( floatValue );
    }

    static long previousMillis = 0;
    long interval = 1000;
    unsigned long currentMillis = millis();
    if ( currentMillis - previousMillis > interval )
    {
      previousMillis = currentMillis;
      static float amplitude = 0.5;
      amplitude += 0.1;
      if ( amplitude > 8.0 )
      {
        amplitude = 0.5;
      }
      Serial.println(amplitude );
      // amplitudeCharacteristic.writeValue( &amplitude, 4 );
    }
  }

  peripheral.disconnect();
  return true;
}