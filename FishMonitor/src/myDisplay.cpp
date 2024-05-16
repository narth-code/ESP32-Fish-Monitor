#include "LCD_Handler.h"
#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "LCD_Icons.h"

Adafruit_SSD1305 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

u_int8_t currentScreen = MAIN_PAGE;

EasyButton leftButton(SW1_PIN);
EasyButton middleButton(SW2_PIN);
EasyButton rightButton(SW3_PIN);
// Create an instance of the struct to hold our flags
ButtonFlags flags = {false, false, false, false};
volatile bool screenOn = true;

extern int delayMinutes;
extern hw_timer_t * timer1;
extern hw_timer_t * timer3;
extern int targetHour, targetMinute;
extern volatile bool allowFeed;
extern volatile bool tFlags[2];
extern BluetoothSerial ESP_BT;
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
  screenOn = true;
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
  display.printf("Temp:%.1fF",data[TEMP]);


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
  display.print("/14");

  // Button indicators
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,SCREEN_HEIGHT-7);
  display.print("Settings");
  display.setTextColor(WHITE);


  display.setTextColor(BLACK, WHITE);
  display.drawLine(52,SCREEN_HEIGHT-7, 52, SCREEN_HEIGHT, WHITE);
  display.setCursor(53,SCREEN_HEIGHT-7);
  display.print("Feed");
  

  display.setTextColor(WHITE);


  display.setTextColor(BLACK, WHITE);
  display.drawLine(81,SCREEN_HEIGHT-7, 81, SCREEN_HEIGHT, WHITE);
  display.setCursor(82,SCREEN_HEIGHT-7);
  display.print("Refresh");
  display.setTextColor(WHITE);


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


  // Button indicators
  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);
  display.setTextColor(WHITE);

  display.setTextColor(BLACK, WHITE);
  display.setCursor(50,SCREEN_HEIGHT-7);
  display.write(0x11); //◄
  display.write("|");
  display.write(0x10); //► 

  display.setTextColor(WHITE);
  display.setTextColor(BLACK, WHITE);
  display.drawLine(85,SCREEN_HEIGHT-7, 85, SCREEN_HEIGHT, WHITE); //This is for the formatting of the box
  display.setCursor(86,SCREEN_HEIGHT-7);
  display.print(F("Select"));
  display.setTextColor(WHITE);


  display.display();
}
// MARK: DISPLAY FEED
void displayFeedPage(){
  

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Time:");
  display.printf("%02d:%02d:%02d",rtc.getHour(true), rtc.getMinute(), rtc.getSecond());
  display.printf(" (%d/14)", (int8_t)data[FOOD_COUNT]);
  int remainingMinutes = (timerStarted(timer1) ? timerReadSeconds(timer1) : timerReadSeconds(timer3)) / 60;


  display.setCursor(0,10);
  display.print("Feed:");
  display.printf("%02d:%02d (in %02d:%02d)" , targetHour % 12, targetMinute, remainingMinutes /60, remainingMinutes % 60);
  
  // Button indicators
  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);
  display.setTextColor(WHITE);


  display.setTextColor(WHITE);
  display.setTextColor(BLACK, WHITE);
  display.drawLine(71,SCREEN_HEIGHT-7, 71, SCREEN_HEIGHT, WHITE); //This is for the formatting of the box
  display.setCursor(72,SCREEN_HEIGHT-7);
  display.print(F("Feed Now"));
  display.setTextColor(WHITE);


  display.display();

}

// MARK: DEBUG
void displayDebug(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  int sec1 = timerReadSeconds(timer1);
  display.print("Timer 1:");
  display.printf("%02d:%02d:%02d", sec1/3600, (sec1/60)%60, sec1 %60);
  display.print((timerAlarmEnabled(timer1) ? "!" : " "));

  int sec2 = timerReadSeconds(timer3);


  display.setCursor(0,10);
  display.print("Timer 3:");
  display.printf("%02d:%02d:%02d", sec2/3600, (sec2/60)%60, sec2 %60);
  display.print((timerAlarmEnabled(timer3) ? "!" : " "));

  display.setCursor(105, 0);
  display.print((timerStarted(timer1) ? ">": " "));
  display.setCursor(105, 10);
  display.print((timerStarted(timer3) ? ">": " "));

  display.setCursor(120, 0);
  display.print((tFlags[0] ? "1": "X"));
  display.setCursor(120, 10);
  display.print((tFlags[1] ? "3": "X"));
  
  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);
  display.setTextColor(WHITE);

  display.display();
}
// MARK: DISPLAY BLUETOOTH
void displayBluetoothSettings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  if (ESP_BT.hasClient()) {
    display.println("Device is connected");
  } else {
    display.println("No device connected");
  }
  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);

  display.display();
}
// MARK: DISPLAY MAINTENANCE
void displayMaintenanceSettings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(13,10);
  display.println(F("Reset Food Count?"));

  display.setTextColor(BLACK, WHITE);
  display.drawBitmap(0, display.height()-7, ireturnArrow, 5, 7, WHITE);
  display.setCursor(6,SCREEN_HEIGHT-7);
  display.print(F("Back"));
  display.drawLine(5, SCREEN_HEIGHT-7, 5, SCREEN_HEIGHT, WHITE);
  
  display.drawLine(91,SCREEN_HEIGHT-7, 91, SCREEN_HEIGHT, WHITE);
  display.setCursor(92,SCREEN_HEIGHT-7);
  display.print(F("RESET"));
  
  display.display();
}


// MARK: toDISPLAY
void toDisplay() {

    switch(currentScreen) {
        case MAIN_PAGE:
            displayMainPage(); break;
        case FEED_PAGE:
            displayFeedPage(); break;
        case SETTINGS_PAGE:
            displaySettingsPage(); break;
        case BLUETOOTH_SETTINGS:
            displayBluetoothSettings(); break;
        case MAINTENANCE_SETTINGS:
            displayMaintenanceSettings(); break;
        case DEBUG_PAGE:
            displayDebug(); break;
        case SCREEN_OFF:
            display.wake();
            break;
        default: break;
    }
    handleButtons(); 
    delayMicroseconds(10);
}

// MARK: handleButtons
void handleButtons() {
    switch(currentScreen) {
        case MAIN_PAGE:
            if (flags.b1) {
                currentScreen = SETTINGS_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) {
                currentScreen = FEED_PAGE;
                flags.b2 = false;
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
        case FEED_PAGE:
            if (flags.b1) { // Return button
                currentScreen = MAIN_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) { 
                currentScreen = DEBUG_PAGE;
                flags.b2 = false;
            }
            if (flags.b3) {
                allowFeed = true; 
                feed();
                flags.b3 = false;
                
                // display.clearDisplay();
                // display.setTextSize(2);
                // display.setTextColor(WHITE);
                // display.setCursor(0,10);
                // display.print("Feeding...");
                // delayMicroseconds(1000);
                //flags.toggleSelect = false;
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
            if (flags.b2) { 
                // Add specific logic here if necessary
                flags.b2 = false;
            }
            if (flags.b3) {
                data[FOOD_COUNT] = 14;
                flags.b3 = false;
                currentScreen = SETTINGS_PAGE;
            }
            break;
        case DEBUG_PAGE:
            if (flags.b1) { // Return button
                currentScreen = FEED_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) { 
                //timerStop(timer1);
                //timerStop(timer3);
                flags.b2 = false;
            }
            if (flags.b3) {
                //timerWrite(timer1,0.5 * 120000);
                //timerStart(timer1);
                flags.b3 = false;
            }
            break;
        case SCREEN_OFF:
            if (flags.b1) {
                currentScreen = MAIN_PAGE;
                flags.b1 = false;
            }
            if (flags.b2) {
                currentScreen = MAIN_PAGE;
                flags.b2 = false;
            }
            if (flags.b3) {
                currentScreen = MAIN_PAGE;
                flags.b3 = false;
            }
            break;
        default:
            break;
    }
}
