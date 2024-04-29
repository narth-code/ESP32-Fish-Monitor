// ButtonHandler.h
#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <ESP32Time.h>
#include <BluetoothSerial.h> 
#include "Sensor_Handler.h"

// Parameters for Bluetooth interface and timing
// int incoming;                          // variable to store byte received from phone 
// int id;                            // received identification byte
// int val_byte1;                     // most significant byte of data value
// int val_byte2;                     // least significant byte of data value
// =========================================

// Time Tracking

//bool time_configured;

//ESP32Time rtc;
//ESP32Time feed_time;
// =========================================



// Function Declarations
void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice);
void setupBT();
void checkBluetooth();

void setTime(int seconds);
void send_values();
void send_BT(int id, int value);
void reset_rx_BT();

void setTime(int minutes);
void setFeedTime(int minutes);
//void IRAM_ATTR setAutoFeed();
//void IRAM_ATTR autoFeed();
#endif
