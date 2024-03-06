// ButtonHandler.h
#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include "BluetoothSerial.h" 
#include "Sensor_Handler.h"

// Parameters for Bluetooth interface and timing
extern int incoming;                           // variable to store byte received from phone 
extern int id;                            // received identification byte
extern int val_byte1;                     // most significant byte of data value
extern int val_byte2;                     // least significant byte of data value

void setupBT();
void checkBluetooth();
void send_BT(int id, int value);
void reset_rx_BT();

#endif
