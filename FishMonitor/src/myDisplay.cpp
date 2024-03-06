#include <Arduino.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>


#include <LCD_Handler.h>




// I2C
Adafruit_SSD1305 display(128, 32, &Wire, OLED_RESET);