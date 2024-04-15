#include <Arduino.h>
#include <Sensor_Handler.h>
#include <Bluetooth_Handler.h>
#include <stepperMotor.h>
#include <secrets.h>
#include <LCD_Handler.h>
#include <gpio_viewer.h>

/*
// Define button pin numbers
const int buttonPins[] = {SW1, SW2, SW3}; // Example pin numbers for SW1, SW2
const int numberOfButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);
ButtonHandler buttons[numberOfButtons] = {
    {SW1, 1},
    {SW2, 2},
    {SW3, 3},
}; // Create ButtonHandler objects for each button
*/

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
  /*
  unsigned long currentMillis = millis();
  if (currentMillis - lastProbeReadTime >= probeReadInterval) {
          lastProbeReadTime = currentMillis;
          readTemp();
          readLevel();
          readPH();
      }
  
// Check for button press actions
  for (int i = 0; i < 3; i++) {
      if (ButtonHandler::actionRequired[i]) {
          ButtonHandler::actionRequired[i] = false; // Reset the flag
          buttons[i].performAction(); // Perform the action associated with the button  
      }
  }
  */
  
  checkBluetooth();
  toDisplay();


}
