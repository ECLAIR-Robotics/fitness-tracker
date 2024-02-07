#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <string>

using namespace std;
// function declaration for platformio
void establishConnection();
void gui();
void establishConnection();
void greenPressed(short* arr);
void save_baseline();
void save_exit();
void writeControl(short action, short person, short exercise, short baseline);
void print_arr(short* arr) ;
void add_shorts_to_byte_array(short* arr, byte* byte_arr, int size);
void displayRecord2() ;
void displayLooking2();
void displayLooking();
void displayDisconnect();
void displayStartRecording();
void displayExerciseMenu() ;
void displayStartMenu();
void displaySecondTop(char* text);
void displayTop(char* text);
void displayRight(char* text);
void displayMiddle(char* text);
void displayLeft(char* text);
void helloWorld();
void initializeDisplay();
void loading();
/* 
 * GUI Vars
 */ 
// constants won't change. They're used here to set pin numbers:
const int GREEN_BUTTON_PIN = 2;       // the number of the green button pin
const int BLUE_BUTTON_PIN = 3;       // the number of the blue button pin
const int RED_BUTTON_PIN = 4;       // the number of the red button pin

// int lastState = LOW;  // the previous state from the input pin
// int currentState; 
int greenState = LOW;
int redState = LOW;
int blueState = LOW;
int globalState = 0; 
/* Here I'll walk through the global state number
0 = begin menu
1 = choose exercise
2 = get ready screen
3 = currently recording screen
*/
int chosenExercise = -1;
// Needs an array of available exercises
char *exercises[] = { "Curl", "Squat",  "Lat Raise"};
int personNum = 1;


/* 
 * BLE Vars
 */ 

BLEService fitnessService("180A"); // BLE fitness Service

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
// represents the service number for IMU
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214"; // random string 
// represents the characteristic number for acceleration

// BLE Acc Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic imuCharacteristic("2A57", BLERead, 18);
BLECharacteristic controlCharacteristic("8AD3", BLERead, 8);
BLEUnsignedLongCharacteristic timeCharacteristic("BC76", BLERead);



/* 
 *  Default display Functions
 */

void loading() {
  SeeedGrayOled.clearDisplay();         // Clear Display.
  for(char i=0; i < 12 ; i+=3)
  {
    SeeedGrayOled.setTextXY(i,0);  //set Cursor to ith line, 0th column
    SeeedGrayOled.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
    SeeedGrayOled.putString("Loading"); //Print Hello World
  }
}

void initializeDisplay(){
  SeeedGrayOled.init(SH1107G);                 // initialize SEEED OLED display
  //SeeedGrayOled.setInverseDisplay();    // Set display to inverse mode
  SeeedGrayOled.clearDisplay();         // Clear Display.
  SeeedGrayOled.setVerticalMode();      // Set to vertical mode for displaying text
}

void helloWorld(){
  SeeedGrayOled.clearDisplay();         // Clear Display.
  for(char i=0; i < 12 ; i++)
  {
    SeeedGrayOled.setTextXY(i,0);  //set Cursor to ith line, 0th column
    SeeedGrayOled.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
    SeeedGrayOled.putString("Hello World"); //Print Hello World
  }
}


void declareAccessories(){
  pinMode(GREEN_BUTTON_PIN, INPUT);
  pinMode(BLUE_BUTTON_PIN, INPUT);
  pinMode(RED_BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  initializeDisplay();
  loading();
  
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
  
  BLE.setLocalName("Nano 33 BLE");
  BLE.setAdvertisedService(fitnessService);

  // add the characteristic to the service
  fitnessService.addCharacteristic(imuCharacteristic);
  fitnessService.addCharacteristic(controlCharacteristic);
  fitnessService.addCharacteristic(timeCharacteristic);

  // add service
  BLE.addService(fitnessService);

  // set the initial value for the characteristics:
  imuCharacteristic.writeValue((byte)0);
  controlCharacteristic.writeValue((byte)0);
  timeCharacteristic.writeValue((byte)0);

  // start advertising
  BLE.advertise();

  Serial.println("Finished Init");
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  declareAccessories();
  displayStartMenu();
}

/* 
 *  GUI Menu printing
 */
void displayLeft(char* text){
  SeeedGrayOled.setTextXY(8,0);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  string out = "R: " + string(text);
  // Serial.println(out.c_str());
  SeeedGrayOled.putString(out.c_str()); //Print Hello World
}

void displayMiddle(char* text){
  SeeedGrayOled.setTextXY(10,1);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  // Serial.println(out.c_str());
  SeeedGrayOled.putString(text); //Print Hello World
}

void displayRight(char* text){
  SeeedGrayOled.setTextXY(12,1);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  // string out = "R: " + string(text);
  // Serial.println(out.c_str());
  SeeedGrayOled.putString(text); //Print Hello World
  
}

void displayTop(char* text){
  SeeedGrayOled.setTextXY(1,0);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(0); //Set Grayscale level. Any number between 0 - 15.
  // Serial.println(text);
  SeeedGrayOled.putString(text); //Print Hello World
}

void displaySecondTop(char* text){
  SeeedGrayOled.setTextXY(5,5);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(0); //Set Grayscale level. Any number between 0 - 15.
  // Serial.println(text);
  SeeedGrayOled.putString(text); //Print Hello World
}

void displayStartMenu() {
  SeeedGrayOled.clearDisplay(); 
  displayRight("Press Green");
}

void displayExerciseMenu() {
  SeeedGrayOled.clearDisplay();         // Clear Display.
  personNum = 1;
  displayTop("Pick Exercise");
  displayLeft(exercises[0]);
  displayMiddle(exercises[1]);
  displayRight(exercises[2]);
}

void displayStartRecording() {
  SeeedGrayOled.clearDisplay();
  if (chosenExercise == -1) {
    displayTop("ERROR");
    return;
  }
  displayTop(exercises[chosenExercise]);
  char cstr[16];
  itoa(personNum, cstr, 10);
  displaySecondTop(cstr);
  displayLeft("Back");
  displayMiddle("Person ++ ");
  displayRight("Start");
}

void displayRecord() {
  SeeedGrayOled.clearDisplay();
  displayTop(exercises[chosenExercise]);
  displaySecondTop("RECORDING *");
  displayLeft("Save & exit");
  displayMiddle("Save baseline");
  displayRight("Save & exit");
  Serial.println("Recording");
}

void displayDisconnect() { 
  SeeedGrayOled.clearDisplay();
  displayTop("DISCONNECTED");
  displaySecondTop("DISCONNECTED");
  displayMiddle("DISCONNECTED");
}

void displayLooking(){
  SeeedGrayOled.clearDisplay();
  displayLeft("Looking");
  displayMiddle("For");
  displayRight("Connection");
}

void displayLooking2(){ //HERE
  // SeeedGrayOled.clearDisplay();
  displayTop(exercises[chosenExercise]);
  displaySecondTop("LOOKING");
  displayLeft("Save & exit");
  displayMiddle("Save baseline");
  displayRight("Save & exit");
  // Serial.println("Recording");
}

void displayRecord2() {
  // displayTop(exercises[chosenExercise]);
  displaySecondTop("RECORDING *");
  // displayLeft("Save & exit");
  // displayMiddle("Save & exit");
  // displayRight("Save & exit");
  Serial.println("Recording");
}


/* 
 *  "Business" logic 
 */

void add_shorts_to_byte_array(short* arr, byte* byte_arr, int size){ 
    // uint32_t val = 0x12345678;   // Value to be written 
    // uint8_t array[24];           // array to which val is to be written
    // uint8_t ptr = &array[14];    // pointer to first byte to be written
    // for (int i=0; i<3; i++)
    // {
    //     ptr[i] = (uint8_t)(val >> (i * 8));  // get one byte of val, convert to unsigned byte, and write to array
    // }
  for (int i = 0; i < size; ++i){
    short val = arr[i];
    int j = i * 2;
    byte_arr[j] = (byte) (val >> 8);
    byte_arr[j+1] = (byte) val;
  }
}

void print_arr(short* arr) {
  for(int i = 0; i < 4; i++)
  {
    Serial.print(arr[i]);
  }
  Serial.println(";");
}

void writeControl(short action, short person, short exercise, short baseline) {
  short arr [] = {action, person, exercise, baseline};
  byte byte_arr [8];
  add_shorts_to_byte_array(arr, byte_arr, 4);
  // print_arr(arr);
  // write to characteristic 
  controlCharacteristic.writeValue(byte_arr, 8);
}

void save_exit(){
  // save and exit
  // for (int i = 0; i < 100; i++){
  //   writeControl(0, personNum, chosenExercise, 0);
  // }
  // writeControl(0, personNum, chosenExercise, 0);
  // writeControl(0, personNum, chosenExercise, 0);
  displayDisconnect();
  globalState = 2;
  displayStartRecording();
}

void save_baseline(){
  // save and exit
  // writeControl(0, personNum, chosenExercise, 1);
  // writeControl(0, personNum, chosenExercise, 1);
  // writeControl(0, personNum, chosenExercise, 1);
  displayDisconnect();
  globalState = 2;
  displayStartRecording();
}

void greenPressed(short* arr){
  arr[0] = 1;
  arr[1] = 0;
  switch(globalState) {
    case 0:
      globalState = 1;
      displayExerciseMenu();
      break;
    case 1:
      globalState = 2;
      chosenExercise = 2;
      displayStartRecording();
      break;
    case 2: 
      // start recording
      globalState = 3;
      
      establishConnection();
      break;
    case 3: 
      arr[0] = 0;
      break;
  }
}

void redPressed(short* arr){
  arr[0] = 1;
  arr[1] = 0;
  switch(globalState) {
    case 0:
      
      break;
    case 1:
      globalState = 2;
      chosenExercise = 0;
      displayStartRecording();
      break;
    case 2: 
      globalState = 1;
      displayExerciseMenu();
      break;
    case 3: 
      arr[0] = 0;
      break;
  }
}

void bluePressed (short* arr){
  arr[0] = 1;
  arr[1] = 0;
  switch(globalState) {
    case 0:
      
      break;
    case 1:
      // globalState = 0;
      globalState = 2;
      chosenExercise = 1;
      displayStartRecording();
      break;
    case 2: 
      personNum = personNum + 1;
      displayStartRecording();
      break;
    case 3: 
      arr[0] = 0;
      arr[1] = 1;
      break;
  }
}

void checkButtons(int currGreen, int currBlue, int currRed, short* arr){
  if(greenState == LOW && currGreen == HIGH){
    Serial.println("green Pressed");
    return greenPressed(arr);
  }
  
  if(redState == LOW && currRed == HIGH){
    Serial.println("red Pressed");
    return redPressed(arr);
  }

  if(blueState == LOW && currBlue == HIGH){
    Serial.println("blue Pressed");
    return bluePressed(arr);
  }
  arr[0] = 1;
  arr[1] = 0;
}

void print_arr(byte* arr) {
  for(int i = 0; i < 18; i++)
  {
    Serial.print(arr[i], HEX);
  }
  Serial.println(";");
}

void record(short* control){
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
  /*
    Accelerometer output data rate is fixed at 104 Hz.
    Gyroscope output data rate is fixed at 104 Hz.
    Magnetometer output data rate is fixed at 20 Hz. 
    The data caputures differ (in terms of timestamps) by 312 "units"
  */
  // poll acc
  if (IMU.accelerationAvailable()) { // values from -128 to 127
    IMU.readAcceleration(fx_a, fy_a, fz_a);
    x_a = fx_a * 100;
    y_a = fy_a * 100;
    z_a = fz_a * 100;
  //   Serial.print(x_a);
  //   Serial.print(',');
  //   Serial.print(y_a);
  //   Serial.print(',');
  //   Serial.print(z_a);
  //   Serial.print(';');
  // }else{
  //   Serial.print("0,0,0;");
  }
  if (IMU.magneticFieldAvailable()) { // values from -180 to 180 ????
    IMU.readMagneticField(fx_m, fy_m, fz_m);
    x_m = (short) floorf(fx_m);
    y_m = (short) floorf(fy_m);
    z_m = (short) floorf(fz_m);
  //   Serial.print(x_m);
  //   Serial.print(',');
  //   Serial.print(y_m);
  //   Serial.print(',');
  //   Serial.print(z_m);
  //   Serial.print(';');
  // }else{
  //   Serial.print("0,0,0;");
  }

  if (IMU.gyroscopeAvailable()) { // values from -128 to 127 
    IMU.readGyroscope(fx_g, fy_g, fz_g);
    x_g = (short) floorf(fx_g);
    y_g = (short) floorf(fy_g);
    z_g = (short) floorf(fz_g);
    // Serial.print(x_g);
    // Serial.print(',');
    // Serial.print(y_g);
    // Serial.print(',');
    // Serial.println(z_g);
  // }else{
  //   Serial.println("0,0,0;");
  }
  short  arr [] = {x_a, y_a, z_a, x_m, y_m, z_m, x_g, y_g, z_g};
  // print_arr(arr);
  add_shorts_to_byte_array(arr, byte_arr, 9);
  // print_arr(byte_arr);
  // write to characteristic 
  imuCharacteristic.writeValue(byte_arr, 18);
  // Serial.println(millis());
  timeCharacteristic.writeValue(millis());
  writeControl(control[0], personNum, chosenExercise, control[1]);
  delay(100);
}

void establishConnection(){
  displayLooking2();
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();
  // if a central is connected to peripheral:
  
  if (central) {
    displayRecord2();


    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);            // turn on the LED to indicate the connection

    bool exit = false;
    short ctrl [2] = {1, 0};
    // while the central is still connected to peripheral:
    while (central.connected()) {
      exit = true;
      record(ctrl);

      int currentGreen = digitalRead(GREEN_BUTTON_PIN);
      int currentBlue = digitalRead(BLUE_BUTTON_PIN);
      int currentRed = digitalRead(RED_BUTTON_PIN);
      checkButtons(currentGreen, currentBlue, currentRed, ctrl);
    }
    if (exit == true){
      save_exit();
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  }
}

void gui() { 
  // read the state of the switch/button:
  int currentGreen = digitalRead(GREEN_BUTTON_PIN);
  int currentBlue = digitalRead(BLUE_BUTTON_PIN);
  int currentRed = digitalRead(RED_BUTTON_PIN);
  
  //Serial.println("READ" + currentState);
  short dummy [2];
  checkButtons(currentGreen, currentBlue, currentRed, dummy);
  
  if (globalState == 3) {
    establishConnection();
  }
  // save the the last state
  currentGreen = digitalRead(GREEN_BUTTON_PIN);
  currentBlue = digitalRead(BLUE_BUTTON_PIN);
  currentRed = digitalRead(RED_BUTTON_PIN);

  greenState = currentGreen;
  blueState = currentBlue;
  redState = currentRed;
  
  // Serial.print("Exercise selected: ");
  // Serial.println(chosenExercise);

  Serial.print("State: ");
  Serial.println(globalState);
  // delay (100);
}


void loop() {
  gui();
}

