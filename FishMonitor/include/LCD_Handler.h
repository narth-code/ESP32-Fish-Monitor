#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET     23
#define SCREEN_ADDRESS 0x3C // OLED display I2C address

#define SW1_PIN 16
#define SW2_PIN 4
#define SW3_PIN 2

#define BAUD 115200
#define DEBOUNCE_MS 200
#define screenDimTime  60000 // 60000 ms = 1 minute

struct ButtonFlags {
  volatile bool b1;
  volatile bool b2;
  volatile bool b3;
  volatile bool toggleSelect;  
};


void setupLCD();
void toDisplay();

void displayMainPage();
void displaySettingsPage();
void displayBluetoothSettings();
void displayMaintenanceSettings();

void handleMainPageButtons();
void handleSettingsPageButtons();
void handleBluetoothPageButtons();
void handleMaintenancePageButtons();
extern void drawArrow(uint8_t x, uint8_t y);
#endif