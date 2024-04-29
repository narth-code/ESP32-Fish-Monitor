#include <Arduino.h>
#include <gpio_viewer.h>
#include <ESP32Time.h>
#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "LCD_Handler.h"
#include "secrets.h"


//extern ESP32Time rtc, feed_time;

unsigned long lastProbeReadTime = 0;
//GPIOViewer viewer;

extern ButtonFlags flags;
extern float data[4];
void setup() {

  Serial.begin(BAUD);
  //viewer.connectToWifi(WIFI_SSID ,WIFI_PASS);
  //viewer.begin();

  setupSensors();
  setupLCD();
  setupBT();
}

void loop() {

  
  checkBluetooth();
  toDisplay();


}
