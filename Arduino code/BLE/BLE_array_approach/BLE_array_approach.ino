/*

*/
#include "IEEE11073float.h"


void setup() {
    
}

void loop() {
  
  /* Supposed to convert a float to a 4 byte int 
  From here: https://github.com/adafruit/Adafruit_BluefruitLE_nRF51/blob/master/examples/healththermometer/healththermometer.ino
  Potential issues discussed here: https://forums.adafruit.com/viewtopic.php?t=115945
    // https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.temperature_measurement.xml
  // Chars value is 1 flag + 4 float value. Tempearature is in Fahrenheit unit
  uint8_t temp_measurement [5] = { bit(0) };
  
  float2IEEE11073(temp, temp_measurement+1);

  // TODO temperature is not correct due to Bluetooth use IEEE-11073 format
  gatt.setChar(htsMeasureCharId, temp_measurement, 5);
  
  */
  
  
  // function to convert back to float from a 4 byte int
  /*
  float IEEE11073_2float(uint8_t *dat)
  {
  int32_t Mantissa = (dat[2] << 16 | dat[1] << 8 | dat[0]);
  uint8_t Neg = bitRead(dat[2],7);
  int8_t fExp = dat[3];
  if (Neg) Mantissa |= 255 << 24;
  return (float(Mantissa) * pow(10, fExp));
  }  
  */
}
