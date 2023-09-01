#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>

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
char *exercises[] = { "B. Curl", "Squat",  "Due"};
int personNum = 1;

void initializeDisplay(){
  SeeedGrayOled.init(SH1107G);                 // initialize SEEED OLED display
  // SeeedGrayOled.setInverseDisplay();    // Set display to inverse mode
  SeeedGrayOled.clearDisplay();         // Clear Display.
  SeeedGrayOled.setVerticalMode();      // Set to vertical mode for displaying text
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
  initializeDisplay();
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  declareAccessories();
  displayStartMenu();
}

void highlightPressed() {
  SeeedGrayOled.clearDisplay();         // Clear Display.
  SeeedGrayOled.setTextXY(5,4);  //set Cursor to ith line, 0th column
  SeeedGrayOled.setGrayLevel(15); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString("PRESSED"); //Print
  Serial.println("The button is pressed");
}

void printReleased() {
  SeeedGrayOled.clearDisplay();         // Clear Display.
  SeeedGrayOled.setTextXY(5,4);  //set Cursor to ith line, 0th column
  SeeedGrayOled.setGrayLevel(0); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString("RELEASED"); //Print Hello World
  Serial.println("The button is released");
}

void displayLeft(char* text){
  SeeedGrayOled.setTextXY(8,0);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(text); //Print Hello World
}

void displayMiddle(char* text){
  SeeedGrayOled.setTextXY(10,5);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(text); //Print Hello World
}

void displayRight(char* text){
  SeeedGrayOled.setTextXY(12,10);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(5); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(text); //Print Hello World
  
}

void displayTop(char* text){
  SeeedGrayOled.setTextXY(1,3);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(0); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(text); //Print Hello World
}

void displaySecondTop(char* text){
  SeeedGrayOled.setTextXY(5,5);  //set Cursor to ith line, jth column
  SeeedGrayOled.setGrayLevel(0); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString(text); //Print Hello World
}
void displayStartMenu() {
  SeeedGrayOled.clearDisplay(); 
  displayRight("Pick Exercises");
}

void displayExerciseMenu() {
  SeeedGrayOled.clearDisplay();         // Clear Display.
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
  displayMiddle("Save & exit");
  displayRight("Save & exit");
  Serial.println("Recording");
}

void record(){
  displayRecord();
  // TODO: actually record
}

void save_exit(){
  // save and exit
  // TODO: save
  
  globalState = 2;
  displayStartRecording();
}


void greenPressed(){
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
      record();
      break;
    case 3: 
      save_exit();
      break;
  }
}

void redPressed(){
  switch(globalState) {
    case 0:
      
      break;
    case 1:
      // globalState = 0;
      globalState = 2;
      chosenExercise = 0;
      displayStartRecording();
      break;
    case 2: 
      globalState = 1;
      displayExerciseMenu();
      break;
    case 3: 
      save_exit();
      break;
  }
}

void bluePressed (){
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
      save_exit();
      break;
  }
}

void loop() {
  // read the state of the switch/button:
  int currentGreen = digitalRead(GREEN_BUTTON_PIN);
  int currentBlue = digitalRead(BLUE_BUTTON_PIN);
  int currentRed = digitalRead(RED_BUTTON_PIN);
  
  //Serial.println("READ" + currentState);
  if(greenState == LOW && currentGreen == HIGH){
    Serial.println("green Pressed");
    greenPressed();
  }
  
  if(redState == LOW && currentRed == HIGH){
    Serial.println("red Pressed");
    redPressed();
  }

  if(blueState == LOW && currentBlue == HIGH){
    Serial.println("blue Pressed");
    bluePressed();
  }
  // save the the last state
  // lastState = currentState;
  greenState = currentGreen;
  blueState = currentBlue;
  redState = currentRed;
  
  Serial.print("Exercise selected: ");
  Serial.println(chosenExercise);

  Serial.print("State: ");
  Serial.println(globalState);
  delay (100);
}
