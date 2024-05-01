#include <Arduino.h>
//#include <gpio_viewer.h>
#include <ESP32Time.h>
#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "LCD_Handler.h"
#include "secrets.h"


//extern ESP32Time rtc, feed_time;

unsigned long lastProbeReadTime;
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
  readSensors();
}

void loop() {
  extern volatile bool allowFeed;
  unsigned long currentMillis = millis();  // Get the current time

  if (currentMillis - lastProbeReadTime >= 60000) {  // Compare the difference to the interval
    lastProbeReadTime = currentMillis;  // Update the last time a reading was taken
    Serial.println("Interval Read");
    readSensors();  // Function to take a reading
  }
  else if ((currentMillis - lastProbeReadTime >= 6000))
  {
    checkBluetooth();

  }
  if(allowFeed)
  {
    feed();
  }
  toDisplay();

}
