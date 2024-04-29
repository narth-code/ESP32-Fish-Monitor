#include <Arduino.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include <EasyButton.h>

#include <LCD_Handler.h>
#include <LCD_Icons.h>
#include <Sensor_Handler.h>


Adafruit_SSD1305 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


enum{
  MAIN_PAGE,
  SETTINGS_PAGE,
  BLUETOOTH_SETTINGS, 
  MAINTENANCE_SETTINGS,
  FEED_PAGE
};
u_int8_t currentScreen= MAIN_PAGE;


EasyButton leftButton(SW1_PIN);
EasyButton middleButton(SW2_PIN);
EasyButton rightButton(SW3_PIN);
// Create an instance of the struct to hold our flags
ButtonFlags flags = {false, false, false, false};


void leftPressed()    
{  
  Serial.println("LEFT pressed");
  flags.b1 = true;
}
void middlePressed()  
{  
  Serial.println("MIDDLE pressed");
  flags.b2 = true;
  //flags.toggleSelect = !(flags.toggleSelect);
}
void rightPressed()   
{  
  Serial.println("RIGHT pressed");
  flags.b3 = true;
}
void IRAM_ATTR buttonISR() {
  leftButton.read();
  middleButton.read();
  rightButton.read();
}
// MARK: SETUP
void setupLCD() {

  while (! Serial) delay(100);
  Serial.println("Initializing SSD1305 OLED...");

  Wire.begin();
  if ( ! display.begin(0x3C) ) {
     Serial.println("Unable to initialize OLED");
     yield();
  }
  Serial.println("OLED Initialized");

  display.display(); // Display initialization
  
  // Set up buttons
  leftButton.begin();
  middleButton.begin();
  rightButton.begin();

  leftButton.onPressed(leftPressed);
  middleButton.onPressed(middlePressed);
  rightButton.onPressed(rightPressed);

  // interupt setup

  leftButton.enableInterrupt(buttonISR);
  middleButton.enableInterrupt(buttonISR);
  rightButton.enableInterrupt(buttonISR);
}
// MARK: DISPLAY MAIN
void displayMainPage() {
  display.clearDisplay();
  display.setTextWrap(false);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Temp:");
  display.print(((int)round(data[TEMP]*10))/10);
  display.print("F");

  display.setCursor(64,0);
  display.print("Level:");
  display.print(data[WATER_LEVEL] > 0 ? "Good" : "Bad");

  display.setCursor(0,10);
  display.print("pH:");
  display.print(data[PH]); 
  //display.print(pHGood ? "Good" : "Bad");



  display.setCursor(64,10);
  display.print("Food:");
  display.print((int)data[FOOD_COUNT]);
  display.print("/15");

  // Button indicators
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,SCREEN_HEIGHT-11);
  display.print("Settings");
  //display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
  display.setTextColor(WHITE);
  //display.print(" ");

  display.setTextColor(BLACK, WHITE);
  //display.drawLine(54,SCREEN_HEIGHT-11, 54, SCREEN_HEIGHT-4, WHITE);
  display.setCursor(52,SCREEN_HEIGHT-11);
  display.print("Feed");
  

  display.setTextColor(WHITE);
  //display.print(" ");

  display.setTextColor(BLACK, WHITE);
  //display.drawLine(85,SCREEN_HEIGHT-11, 85, SCREEN_HEIGHT-4, WHITE);
  display.setCursor(82,SCREEN_HEIGHT-11);
  display.print("Refresh");
  display.setTextColor(WHITE);
  //drawArrow(106,31);
          //display.drawRoundRect(0,0,128,32,0,WHITE);

  display.display();
}
// MARK: DISPLAY SETTINGS
void displaySettingsPage() {
  display.clearDisplay();
  if(flags.toggleSelect) {
    //draw box around wrench
    display.drawRoundRect(96, 2, 22, 22, 8, WHITE);
  }
  else {
    // draw box around bluetooth
    display.drawRoundRect(62, 2, 22, 22, 8, WHITE);
  }
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,0);
  display.print(F("Settings"));

  display.drawBitmap(65, 5, bluetooth_icon, 16, 16, WHITE);
  display.drawBitmap(100, 5, wrench_icon, 15, 15, WHITE);
  /*
  display.setTextColor(WHITE);
  display.setCursor(55,0);
  display.print(F("Bluetooth"));

  display.setCursor(55,10);
  display.print(F("Maintenance"));
  */

  // Button indicators
  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);
  display.setTextColor(WHITE);

  display.setTextColor(BLACK, WHITE);
  //display.drawLine(54,SCREEN_HEIGHT-11, 54, SCREEN_HEIGHT-4, WHITE);
  display.setCursor(50,SCREEN_HEIGHT-7);
  display.write(0x11); //◄
  display.write("|");
  display.write(0x10); //► 

  display.setTextColor(WHITE);
  display.setTextColor(BLACK, WHITE);
  //display.drawLine(85,SCREEN_HEIGHT-11, 85, SCREEN_HEIGHT-4, WHITE); //This is for the formatting of the box
  display.setCursor(86,SCREEN_HEIGHT-7);
  display.print(F("Select"));
  display.setTextColor(WHITE);
  //drawArrow(106,31);

  display.display();
}
// MARK: DISPLAY BLUETOOTH
void displayBluetoothSettings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("Connect ('Connected')"));
  display.println(F("Disconnect"));
  display.display();
}
// MARK: DISPLAY MAINTENANCE
void displayMaintenanceSettings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("Reset Food Count"));
  display.println(F("Clear Notification"));


  

  display.clearDisplay();
  display.drawBitmap(0, 0, MANTA_LOGO, 128, 32, WHITE);

  
  display.display();
}
// MARK: toDISPLAY
void toDisplay() {

    switch(currentScreen) {
        case MAIN_PAGE:
            displayMainPage(); break;
        case SETTINGS_PAGE:
            displaySettingsPage(); break;
        case BLUETOOTH_SETTINGS:
            displayBluetoothSettings(); break;
        case MAINTENANCE_SETTINGS:
            displayMaintenanceSettings(); break;
        default: break;
    }
    handleButtons(); 
}

// MARK: handleBUttons
void handleButtons() {
    switch(currentScreen) {
        case MAIN_PAGE:
            if (flags.b1) {
                currentScreen = SETTINGS_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) {
                flags.b2 = false;
                feed();
            }
            if (flags.b3) {
                readSensors();
                flags.b3 = false;
            }
            break;

        case SETTINGS_PAGE:
            if (flags.b1) { // Return button
                currentScreen = MAIN_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) { // Navigate
                flags.toggleSelect = !(flags.toggleSelect);
                flags.b2 = false;
            }
            if (flags.b3) {
                currentScreen = (flags.toggleSelect ? MAINTENANCE_SETTINGS : BLUETOOTH_SETTINGS);
                flags.b3 = false;
                flags.toggleSelect = false;
            }
            break;

        case BLUETOOTH_SETTINGS:
            if (flags.b1) { // Return button
                currentScreen = SETTINGS_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) { // Navigate
                // Add specific logic here if necessary
                flags.b2 = false;
            }
            if (flags.b3) {
                // Implement refresh logic here
                flags.b3 = false;
            }
            break;

        case MAINTENANCE_SETTINGS:
            if (flags.b1) { // Return button
                currentScreen = SETTINGS_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) { // Navigate
                // Add specific logic here if necessary
                flags.b2 = false;
            }
            if (flags.b3) {
                // Implement refresh logic here
                flags.b3 = false;
            }
            break;

        default:
            break;
    }
}
void drawArrow(uint8_t x, uint8_t y)
{
  display.drawLine(x, y, x+2, y, WHITE);
  display.drawPixel(x+1, y-1, WHITE);
}