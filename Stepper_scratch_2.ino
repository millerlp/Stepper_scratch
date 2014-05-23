/*  Stepper motor example with Big Easy Driver
    Connect ground, pin 8 + 9 to BED

    Motor Red + Blue to A
    Motor Green + Black to B
*/
int Distance = 0;  // Record the number of steps we've taken
//#define FULLSTEP
#define rxPin 4
const byte stepperDir = 8;  // stepper direction pin
const byte stepperStep = 9; // stepper step pin
const byte stepperEnable = 12; // Big easy driver enable pin
const byte button1 = 13; // override button
const byte trigger = 7; // camera trigger button
int triggerLength = 1500; // shutter time in milliseconds
unsigned int totalSteps = 0; // keep track of total steps
// assume one full rotation takes ~ 1000ms.


#ifdef FULLSTEP
const int stepDelay = 5000; // delay for full step mode, microseconds
// A value around 5000 seems to produce the best power.
unsigned int stepVal = 200; // number of steps per button press
#else
const int stepDelay = 200; // delay for 1/16 step mode, microseconds
// A value around 200-250 seems to produce the best power.
unsigned int stepVal = 3200; // number of steps per button press
#endif

void setup() {
  Serial.begin(115200);
  pinMode(stepperDir, OUTPUT);
  pinMode(stepperStep, OUTPUT);
  pinMode(stepperEnable, OUTPUT);
  pinMode(trigger, OUTPUT);
  pinMode(rxPin, OUTPUT);
  pinMode(button1, INPUT);
#ifdef FULLSTEP
  Serial.println("Full step mode");
  digitalWrite(rxPin, LOW); // full step mode
#else
  Serial.println("Microstep mode");
  digitalWrite(rxPin, HIGH); // microstep mode
#endif
  digitalWrite(stepperEnable, HIGH);  // disables stepper when high
  digitalWrite(stepperDir, LOW);
  digitalWrite(stepperStep, LOW);
  digitalWrite(button1, HIGH); // turn on internal pull-up resistor
  //  delay(2000);

}


void loop() {
  // Use button to return rack to home.
  // Then set off on a transit, with a step followed by a shutter trigger
  // Only travel a set number of steps.
  // Rack is 10tpi, so each rotation = 1/10 inch
  // conservatively allow for 45 inches of travel = 450 full rotations
  // then stop.
  while (totalSteps < 450) {
    if (digitalRead(button1) != HIGH) {
      rewindFunction();
    }
    delay(100);
    digitalWrite(stepperEnable, LOW); // enable motor
    digitalWrite(stepperDir, HIGH);
    for (unsigned int steps = 0; steps < stepVal; steps++) {
      digitalWrite(stepperStep, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepperStep, LOW);
    }
    digitalWrite(stepperEnable, HIGH); // turn off motor
    delay(500); // pause to let camera stabilize
    digitalWrite(trigger, HIGH); // trigger shutter
    delay(triggerLength);
    digitalWrite(trigger, LOW);
    totalSteps = totalSteps++; // increment totalSteps

    if (digitalRead(button1) != HIGH) {
      rewindFunction();
    }

  }




} // end main loop


void rewindFunction(void) {
  // When the button is pressed, move the stepper
  if (digitalRead(button1) != HIGH) {
    while (digitalRead(button1 != HIGH)) {
      Serial.println("Moving");
      digitalWrite(stepperEnable, LOW); // enable motor
      digitalWrite(stepperDir, LOW);
      delay(10);
      for (unsigned int steps = 0; steps < stepVal; steps++) {
        digitalWrite(stepperStep, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepperStep, LOW);
      } // end for statement
      Serial.print("Moved ");
      Serial.print(stepVal);
      Serial.println(" steps");
    }
  } // end if statement
  //  delay(100);
  digitalWrite(stepperEnable, HIGH); // turn off motor
}
