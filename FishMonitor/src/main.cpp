#include <Arduino.h>
#include <ESP32Time.h>

#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "LCD_Handler.h"

#include "secrets.h"
#include "debug.h"



//extern ESP32Time rtc, feed_time;


unsigned long lastProbeReadTime, lastScreenTime;

extern ButtonFlags flags;
extern float data[4];
void setup() {

  Serial.begin(BAUD);


  setupSensors();
  setupLCD();
  setupBT();
  readSensors();
}

void loop() {
  extern volatile bool allowFeed, screenOn;

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
  if(screenOn){ // keep track how long screen has been on using flag from buttonISR
    lastScreenTime = currentMillis;
    screenOn = false;
  }
  if(currentMillis - lastScreenTime <= 60000){
    toDisplay();
  }
  else{
    display.clearDisplay();
    display.display();
    display.sleep();
    
    currentScreen = SCREEN_OFF;
  }


  if(allowFeed)
  {
    autoFeed();
  }

  delayMicroseconds(10);
}
