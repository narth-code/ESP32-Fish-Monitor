#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

/* Board Files Header */
#include <Arduino.h>

/* Peripheral Headers */
#include "OneWire.h"
#include "DallasTemperature.h"


//  WATER TEMP========================================
#define ONE_WIRE_BUS 33  // 36 Data wire is connected to GPIO 34

extern OneWire oneWire;

void setupSensors();
extern float readTemp();

//     PH   =========================================
#define pH_PIN  32 //39
#define VOLTAGE 3.3

//void setupPH();
//float readPH();


// WATER LEVEL =========================================
#define SIGNAL_PIN 35     //34  ESP32 pin connected to sensor's signal pin
#define POWER_PIN  4     //35  ESP32 pin connected to sensor's VCC pin

enum{TEMP, PH, WATER_LEVEL, FOOD_COUNT};
extern float data[4]; // variable to store the sensor value

void setupWaterLevel();
extern int readLevel();


void readSensors(); 
#endif