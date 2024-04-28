#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

/* Board Files Header */
#include <Arduino.h>

/* Peripheral Headers */
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Stepper.h>

//  WATER TEMP
#define ONE_WIRE_BUS 35  // Data wire is connected to GPIO 34
extern OneWire oneWire;
//  =========================================

//  PH   
#define pH_PIN  32 //
#define VOLTAGE 3.27
//  =========================================

//  WATER LEVEL 
#define SIGNAL_PIN 33     //   ESP32 pin connected to sensor's signal pin
//#define POWER_PIN  4     //    ESP32 pin connected to sensor's VCC pin
//  =========================================

// STEPPER
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

#define stepsPerRevolution  2048
#define stepsPerSegment(segments)  ((int)stepsPerRevolution / (int)segments)

#define ONE_SERVING stepsPerSegment(15)
//  =========================================


enum{TEMP, PH, WATER_LEVEL, FOOD_COUNT};
extern float data[4]; // variable to store the sensor value


// Function Declaration
void setupSensors();

void readSensors(); 
void feed();

void IRAM_ATTR setAutoFeed();
void IRAM_ATTR autoFeed();
#endif