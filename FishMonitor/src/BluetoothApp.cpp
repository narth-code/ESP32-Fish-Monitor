// --------------------------------------------------
//
// Code for control of ESP32 through MIT inventor app (Bluetooth). 
// device used for tests: ESP32-WROOM-32D
// 
// App on phone has three buttons:
// Button 1: 11 for ON and 10 for OFF
// Button 2: 21 for ON and 20 for OFF
// Button 3: 31 for ON and 30 for OFF
//
// Written by mo thunderz (last update: 20.4.2021)
//
// --------------------------------------------------

// this header is needed for Bluetooth Serial -> works ONLY on ESP32
#include "Bluetooth_Handler.h"

// init Class:
BluetoothSerial ESP_BT; 
ESP32Time rtc;
ESP32Time feed_time;

hw_timer_t * timer1 = NULL;
hw_timer_t * timer3 = NULL;
portMUX_TYPE timerMux3 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
int targetHour, targetMinute;
// timer1 = NULL;
// timer3 = NULL;
// timerMux3 = portMUX_INITIALIZER_UNLOCKED;
// timerMux1 = portMUX_INITIALIZER_UNLOCKED;

int incoming;                           // variable to store byte received from phone 
int id = -1;                            // received identification byte
int val_byte1 = -1;                     // most significant byte of data value
int val_byte2 = -1;  
volatile bool allowFeed = false;



void setupBT() {
  ESP_BT.begin(">FishMonitor");        // Name of your Bluetooth interface -> will show up on your phone
  //Serial.println("3C:E9:0E:08:F9:AE"); // MAC ADDRESS
  Serial.println(">Bluetooth ready to pair");
  
  #ifdef DEBUG
    uint8_t mac_arr[6]; // Byte array to hold the MAC address from getBtAddress()
    String mac_str; // String holding the text version of MAC in format AA:BB:CC:DD:EE:FF
    ESP_BT.getBtAddress(mac_arr); // Fill in the array
    mac_str = ESP_BT.getBtAddressString(); // Copy the string
    Serial.print(">The mac address using string: "); Serial.println(mac_str.c_str());
  #endif

  timer1 = timerBegin(1, 40000, true); // Timer 0, prescaler 80, count up
  timer3 = timerBegin(3, 40000, true);

  timerAttachInterrupt(timer1, &setAutoFeed, true); // Attach interrupt function
  timerAttachInterrupt(timer3, &autoFeed, true); // Attach interrupt function
}



void checkBluetooth() {
  
  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) {
    incoming = ESP_BT.read();           // Read what we receive and store in "incoming"
    //Serial.println(incoming);
    if (incoming > 127) {               // ID bytes are 128 or higher, so check if incoming byte is an ID-byte
      reset_rx_BT();                    // reset id and data to -1
      id = incoming - 128;              // write id value
    }
    else if (val_byte1 == -1) {         // if incoming byte is lower than 128 it is a data byte. Check if first data byte is empty (-1)
      val_byte1 = incoming;             // write first data byte (MSB)
    }
    else if (val_byte2 == -1) {         // check if second data byte is empty (-1)
      val_byte2 = incoming;             // write second data byte (LSB)
      // recombine the first and second data byte to the actual value that was sent from the phone
      float value =128*val_byte1 + val_byte2;
      //float value = (id == 5 || id == 7 ? 128*val_byte1 + val_byte2 : 0); // if it's time id, set the "seconds" value      
      switch (id) {
          case 4: 
            allowFeed = true; 
            feed(); 
            break;
          case 5: setFeedTime(value); // set Time
            break;
          case 6: send_values(); 
            break;
          case 7: setTime(value); // set feeding time
            break;
          default: break;
        }
      Serial.print(">Id: "); Serial.print(id); Serial.print(", val: "); Serial.println(value);   // for debugging write to the serial interface (check with serial monitor)
      //send_BT(id, value);                 // for test purposes we just send the data back to the phone
      //reset_rx_BT();                    // not strictly needed, but just in case erase all bytes (set to -1)
    }
  }
}
void setTime(int minutes) {
  int hours = minutes / 60;
  int mins = minutes % 60;

  rtc.setTime(0, mins, hours, 1, 1, 2024); // Update RTC with new time, assume seconds are 0
  Serial.printf("Time updated to: %02d:%02d\n", hours, mins);
}
// MARK: TIME
void setFeedTime(int minutes) {
  targetHour = minutes / 60;
  targetMinute = minutes % 60;

  int currentHour = rtc.getHour(true);
  int currentMinute = rtc.getMinute();

  // Calculate delay until next feed time in minutes
  int currentTotalMinutes = currentHour * 60 + currentMinute;
  int delayMinutes;
  Serial.print("current minutes"); Serial.println(currentTotalMinutes);
  Serial.print("target minutes"); Serial.println(minutes);
  //delayMinutes = minutes - currentTotalMinutes;
  delayMinutes = (delayMinutes < 0? (currentTotalMinutes - minutes) + 720 : minutes - currentTotalMinutes);
  Serial.print("delay minutes"); Serial.println(delayMinutes);
  if(delayMinutes < 0){
    delayMinutes = (currentTotalMinutes - minutes) + 720;
  }
  
  timerAlarmWrite(timer1, delayMinutes * 120000, false);
  //timerAlarmWrite(timer1, delayMinutes * 60000000, false); // Set timer for the delay in microseconds (1 minute = 60,000,000 microseconds)
  timerAlarmEnable(timer1); // Enable timer

  
  //Serial.printf("Next feeding time set in %d minutes\n", delayMinutes);
  Serial.printf("Next feeding time set for %02d:%02d (in %d minutes)\n", targetHour, targetMinute, delayMinutes);
}


void IRAM_ATTR setAutoFeed() {
  portENTER_CRITICAL_ISR(&timerMux1);
    allowFeed = true;
    timerAlarmWrite(timer3, 1440000, true); // 12 hours in microseconds, auto-reload true 43 200 000 000
    timerAlarmEnable(timer3);
  portEXIT_CRITICAL_ISR(&timerMux1);
}

void IRAM_ATTR autoFeed()
{
  portENTER_CRITICAL_ISR(&timerMux3);
    allowFeed = true;
  portEXIT_CRITICAL_ISR(&timerMux3);
}
void send_values(){
  readSensors();
  for(int i = 0; i <= 3; i++)
  {
    Serial.print(">Id: "); Serial.print(i+1); Serial.print(", val: "); Serial.println(data[i]);
    send_BT(i+1, data[i]);
  }
}

void reset_rx_BT() {                    // function to erase all bytes (set to -1)
  id = -1;
  val_byte1 = -1;
  val_byte2 = -1;
}
void send_BT(int id, int value) {       // function to write id and value to the bluetooth interface (and split value in MSB and LSB
  ESP_BT.write(128 + id);
  ESP_BT.write((value/128));            // MSB
  ESP_BT.write(value%128);              // LSB
}
