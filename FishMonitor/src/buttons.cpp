// ButtonHandler.cpp
#include "Button_Handler.h"

ButtonHandler* ButtonHandler::instances[3] = {nullptr}; // Initialize static array
volatile bool ButtonHandler::actionRequired[3] = {false};


ButtonHandler::ButtonHandler(int buttonPin, int buttonId) : button(buttonPin), pin(buttonPin), id(buttonId) {
    instances[id - 1] = this; // Assign this instance to the static array
}

void ButtonHandler::setupButton() {
    button.setDebounceTime(50); // Set debounce time to 50 milliseconds
    pinMode(pin, INPUT_PULLUP); // Initialize the button pin as an input with internal pull-up resistor
    attachInterrupt(digitalPinToInterrupt(pin), isr, FALLING); // Attach interrupt
}

void ButtonHandler::isr() {
 for (int i = 0; i < 3; i++) {
        if (instances[i] != nullptr) {
            instances[i]->button.loop(); // MUST call the loop() function first
            if (instances[i]->button.isPressed()) {
                actionRequired[i] = true; // Set a flag to indicate action is required
            }
        }
    }
}

void ButtonHandler::performAction() {
    // Perform action based on button ID
    switch(id) {
        case 1:
          readTemp();
          Serial.println("SW1 Pressed!");
          break;
        case 2:
          readLevel();
          Serial.println("SW2 Pressed!");
          break;
        case 3:
          readPH();
          Serial.println("SW3 Pressed!");
          break;           
        default:
            // Default action or error handling
            break;
    }
}
