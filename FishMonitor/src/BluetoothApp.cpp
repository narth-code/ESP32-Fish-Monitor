#include "Bluetooth_Handler.h"

// init Class:
BluetoothSerial ESP_BT; 
ESP32Time rtc;


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
volatile bool tFlags[2] = {false};



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

  timer1 = timerBegin(1, 40000, false); // Timer 0, prescaler 80, count down
  timer3 = timerBegin(3, 40000, false); // count down

  timerStop(timer1);
  timerStop(timer3);

  timerWrite(timer1,0);
  timerWrite(timer3, 120000 * 60 * 12);

  timerAttachInterrupt(timer1, &startFeedTimer, true); // Attach interrupt function
  timerAttachInterrupt(timer3, &onFeedTimer, true); // Attach interrupt function

  timerAlarmWrite(timer1, 0, false);// write value for alarm to trigger, no auto-reload
  timerAlarmWrite(timer3, 0, true); // write value for alarm to trigger, auto-reload

  timerAlarmEnable(timer1);
  timerAlarmEnable(timer3);
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
  int delayMinutes= calculateShortestDelay((currentHour * 60 + currentMinute), minutes);
  
  
  // Serial.print("current minutes"); Serial.println(currentTotalMinutes);
  // Serial.print("target minutes"); Serial.println(minutes);
  
  // delayMinutes = ((minutes - currentTotalMinutes) < 0 ? (currentTotalMinutes - minutes) + 720 : minutes - currentTotalMinutes);
  // Serial.print("delay minutes"); Serial.println(delayMinutes);
  // if(delayMinutes < 0){
  //   delayMinutes = (currentTotalMinutes - minutes) + 720;
  // }
  timerStop(timer3); 
  timerWrite(timer3, 120000 * 60 * 12); // reset 12hr value

  timerWrite(timer1, delayMinutes * 120000);
  timerAlarmEnable(timer1); // since reload is off, the alarm will not retrigger if new value is written into 
  timerStart(timer1);


  //Serial.printf("Next feeding time set for %02d:%02d (in %d minutes)\n", minutes/60, minutes%60, delayMinutes);
}

int calculateShortestDelay(int currentTotalMinutes, int targetTotalMinutes) {
    // Calculate forward delay to the target time
    int forwardDelay = (targetTotalMinutes - currentTotalMinutes + 1440) % 1440;

    // Calculate forward delay to the target time plus 12 hours (720 minutes)
    int targetPlus12Hours = (targetTotalMinutes + 720) % 1440;
    int forwardDelayPlus12Hours = (targetPlus12Hours - currentTotalMinutes + 1440) % 1440;

    // Return the smallest of the two delays
    return min(forwardDelay,forwardDelayPlus12Hours);
}
void IRAM_ATTR startFeedTimer() {
  portENTER_CRITICAL_ISR(&timerMux1);
    tFlags[0] = true;

    timerStop(timer1);
    timerRestart(timer1); // Restarting writes timer value to 0
    
    allowFeed = true;
    timerStart(timer3);

  portEXIT_CRITICAL_ISR(&timerMux1);
}

void IRAM_ATTR onFeedTimer()
{
  portENTER_CRITICAL_ISR(&timerMux3);
    tFlags[1] = true;
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
