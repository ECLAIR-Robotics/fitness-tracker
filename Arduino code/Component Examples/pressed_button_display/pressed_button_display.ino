#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>

// constants won't change. They're used here to set pin numbers:
const int GREEN_BUTTON_PIN = 2;       // the number of the green button pin
const int BLUE_BUTTON_PIN = 3;       // the number of the blue button pin
const int RED_BUTTON_PIN = 4;       // the number of the red button pin

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState; 

void initializeDisplay(){
  SeeedGrayOled.init(SH1107G);                 // initialize SEEED OLED display
  SeeedGrayOled.setInverseDisplay();    // Set display to inverse mode
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

void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(GREEN_BUTTON_PIN);
  //Serial.println("READ" + currentState);
  if(lastState == LOW && currentState == HIGH){
    highlightPressed();
  }else if(lastState == HIGH && currentState == LOW){
    printReleased();
  }
  // save the the last state
  lastState = currentState;
}
