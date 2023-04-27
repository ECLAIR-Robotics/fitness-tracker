/*
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-button
 */

// constants won't change. They're used here to set pin numbers:
const int BUTTON_PIN = 2;       // the number of the pushbutton pin

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;                // the current reading from the input pin

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an pull-up input
  // the pull-up input pin will be HIGH when the switch is open and LOW when the switch is closed.
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);
  //Serial.println("READ" + currentState);
  if(lastState == LOW && currentState == HIGH)
    Serial.println("The button is pressed");
  else if(lastState == HIGH && currentState == LOW)
    Serial.println("The button is released");

  // save the the last state
  lastState = currentState;
}


/* The https://makeabilitylab.github.io/physcomp/arduino/buttons.html#intro-to-digital-input guide

const int INPUT_BUTTON_PIN = 2;
void setup()
{
  pinMode(INPUT_BUTTON_PIN, INPUT);
  Serial.begin(9600); // for printing values to console
}

void loop()
{
  int buttonVal = digitalRead(INPUT_BUTTON_PIN); // returns 0 (LOW) or 1 (HIGH)
  Serial.println(buttonVal);                     // print value to Serial
  delay(5);                                      // small delay
}
*/


