#include <Arduino.h>
#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "Button_Handler.h"
#include "stepperMotor.h"

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
#define probeReadInterval  60000 // 60000 ms = 1 minute

struct Button {
	const uint8_t PIN;
	bool pressed;
};

Button button1 = {SW1, false};

void IRAM_ATTR isr() {
	button1.pressed = true;
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  setupMotor();
  setupSensors();
  setupBT();
  pinMode(button1.PIN, INPUT_PULLUP);
	attachInterrupt(button1.PIN, isr, FALLING);


  //setupPHSensor();
  //setupStepperMotor();
  //setupLCD();
  /*
  for (int i = 0; i < numberOfButtons; i++) {
      buttons[i].setupButton(); // Setup each button (attach interrupt)
  }
  Serial.println("Setup Finish");
  */
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
  if (button1.pressed) {
    feed();
    button1.pressed = false;
  }

}
