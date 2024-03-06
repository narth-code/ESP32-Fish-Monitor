
#include "stepperMotor.h"


Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

void setupMotor(){
  myStepper.setSpeed(10);
}

void feed() {
  // step one segment:
  Serial.print("Moving one segment: "); Serial.println(ONE_SERVING);
  myStepper.step(ONE_SERVING);
  //myStepper.step(2048);

  // You might want to add code here to do something between segments
  // Or simply remove the delay if continuous movement is desired
}
