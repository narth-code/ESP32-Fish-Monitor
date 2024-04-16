#include <Arduino.h>
#include <Sensor_Handler.h>
#include <Bluetooth_Handler.h>
#include <stepperMotor.h>
#include <secrets.h>
#include <LCD_Handler.h>
#include <gpio_viewer.h>



unsigned long lastProbeReadTime = 0;
GPIOViewer viewer;

extern ButtonFlags flags;
extern float data[4];
void setup() {

  Serial.begin(BAUD);
  viewer.connectToWifi(WIFI_SSID ,WIFI_PASS);
  viewer.begin();

  setupMotor();
  setupSensors();
  setupBT();
  setupLCD();

  

}

void loop() {

  
  checkBluetooth();
  toDisplay();


}
