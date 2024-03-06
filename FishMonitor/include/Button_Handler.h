// ButtonHandler.h
#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H


#include <ezButton.h>
#include "Sensor_Handler.h"

#define SW1 2
#define SW2 39
#define SW3 34

class ButtonHandler {
public:
    ButtonHandler(int buttonPin, int buttonId); // Constructor initializes the button and ID
    void setupButton(); // Set up the button, configure it for input and attach the interrupt
    static void isr(); // The ISR to handle button press
    void performAction(); // The action to perform when a button is pressed
    static volatile bool actionRequired[3]; // Flags to indicate required action
    int getPin() const { return pin; } // Getter for pin number

private:
    ezButton button; // ezButton object
    int pin; // Button pin number
    int id; // Button ID
    static ButtonHandler* instances[3]; // Static array to hold instances for ISR access
};

// Extern declaration for the buttons array and its size
extern const int buttonPins[];
extern const int numberOfButtons;
extern ButtonHandler buttons[];



#endif
