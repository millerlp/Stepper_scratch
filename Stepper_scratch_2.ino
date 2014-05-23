/*  Stepper motor example with Big Easy Driver
    Connect ground, pin 8 + 9 to BED
*/
int Distance = 0;  // Record the number of steps we've taken
//#define FULLSTEP
#define rxPin 4
const byte stepperDir = 8;  // stepper direction pin
const byte stepperStep = 9; // stepper step pin
const byte stepperEnable = 12; // Big easy driver enable pin
const byte button1 = 13; // override button
const byte trigger = 7; // camera trigger button


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
  // When the button is pressed, move the stepper
  if (digitalRead(button1) != HIGH) {
    Serial.println("Moving");
    digitalWrite(stepperEnable, LOW); // enable motor
    delay(100);
    for (unsigned int steps = 0; steps < stepVal; steps++){
      digitalWrite(stepperStep, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepperStep, LOW);
    } // end for statement
    Serial.print("Moved ");
    Serial.print(stepVal);
    Serial.println(" steps");
  } // end if statement
//  delay(100);
  digitalWrite(stepperEnable, HIGH); // turn off motor

} // end main loop
