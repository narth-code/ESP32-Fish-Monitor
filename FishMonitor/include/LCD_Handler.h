#ifndef LCD_HANDLER_H
#define LCD_HANDLER_H

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <EasyButton.h>




#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET     23    // SCL = 22, SDA = 21
#define SCREEN_ADDRESS 0x3C // OLED display I2C address

#define SW1_PIN 15  
#define SW2_PIN 16 
#define SW3_PIN 17 

#define BAUD 115200
#define DEBOUNCE_MS 200
#define screenDimTime  60000 // 60000 ms = 1 minute

extern Adafruit_SSD1305 display;

struct ButtonFlags {
  volatile bool b1;
  volatile bool b2;
  volatile bool b3;
  volatile bool toggleSelect;  
};

enum{
  MAIN_PAGE,
  SETTINGS_PAGE,
  BLUETOOTH_SETTINGS, 
  MAINTENANCE_SETTINGS,
  FEED_PAGE,
  DEBUG_PAGE,
  SCREEN_OFF
};
extern u_int8_t currentScreen;

void setupLCD();
void toDisplay();

void displayMainPage();
void displaySettingsPage();
void displayFeedPage();
void displayBluetoothSettings();
void displayMaintenanceSettings();
void displayDebug();
void handleButtons();
// void handleMainPageButtons();
// void handleSettingsPageButtons();
// void handleBluetoothPageButtons();
// void handleMaintenancePageButtons();
void IRAM_ATTR buttonISR();
#endif