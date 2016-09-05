#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 5, 3, 4, 2);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(5);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  //Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);  //// 2048 per revolution
  delay(500);

 // step one revolution in the other direction:
  //Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}
