#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

/* Board Files Header */
#include <Arduino.h>

/* Peripheral Headers */
#include "OneWire.h"
#include "DallasTemperature.h"


//  WATER TEMP========================================
#define ONE_WIRE_BUS 4  // Data wire is connected to GPIO 34

extern OneWire oneWire;
extern DallasTemperature sensors;

void setupSensors();
extern float readTemp();

//     PH   =========================================
#define pH_PIN 13
#define VOLTAGE 3.3

//void setupPH();
//float readPH();


// WATER LEVEL =========================================
#define POWER_PIN  14 // ESP32 pin connected to sensor's VCC pin
#define SIGNAL_PIN 27 // ESP32 pin connected to sensor's signal pin

enum{TEMP, PH, WATER_LEVEL, FOOD_COUNT};
extern float data[4]; // variable to store the sensor value

void setupWaterLevel();
extern int readLevel();


void readSensors(); 
#endif