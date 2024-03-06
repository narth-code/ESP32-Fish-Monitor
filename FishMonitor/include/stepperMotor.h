#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include "Sensor_Handler.h"
#include <Stepper.h>

#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26

#define stepsPerRevolution  2048
#define stepsPerSegment(segments)  ((int)stepsPerRevolution / (int)segments)

#define ONE_SERVING stepsPerSegment(14)

void setupMotor();
void feed();



#endif