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
#include "stepperMotor.h"

// init Class:
BluetoothSerial ESP_BT; 

 int incoming;                           // variable to store byte received from phone 
 int id = -1;                            // received identification byte
 int val_byte1 = -1;                     // most significant byte of data value
 int val_byte2 = -1;  


void setupBT() {
  ESP_BT.begin("ESp32_FishMonitor");        // Name of your Bluetooth interface -> will show up on your phone
  Serial.println("Bluetooth ready to pair");
  
  uint8_t mac_arr[6]; // Byte array to hold the MAC address from getBtAddress()
  BTAddress mac_obj; // Object holding instance of BTAddress with the MAC (for more details see libraries/BluetoothSerial/src/BTAddress.h)
  String mac_str; // String holding the text version of MAC in format AA:BB:CC:DD:EE:FF

  ESP_BT.getBtAddress(mac_arr); // Fill in the array
  mac_obj = ESP_BT.getBtAddressObject(); // Instantiate the object
  mac_str = ESP_BT.getBtAddressString(); // Copy the string

  Serial.print("This device is instantiated with name "); Serial.println("ESp32_FishMonitor");

  Serial.print("The mac address using byte array: ");
  for(int i = 0; i < ESP_BD_ADDR_LEN-1; i++){
    Serial.print(mac_arr[i], HEX); Serial.print(":");
  }
  Serial.println(mac_arr[ESP_BD_ADDR_LEN-1], HEX);

  Serial.print("The mac address using BTAddress object using default method `toString()`: "); Serial.println(mac_obj.toString().c_str());
  Serial.print("The mac address using BTAddress object using method `toString(true)`\n\twhich prints the MAC with capital letters: "); Serial.println(mac_obj.toString(true).c_str()); // This actually what is used inside the getBtAddressString()

  Serial.print("The mac address using string: "); Serial.println(mac_str.c_str());
}



void checkBluetooth() {
 
  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) {
    incoming = ESP_BT.read();           // Read what we receive and store in "incoming"

    if (incoming > 127) {               // ID bytes are 128 or higher, so check if incoming byte is an ID-byte
      reset_rx_BT();                    // reset id and data to -1
      id = incoming - 128;              // write id value
    }
    else if (val_byte1 == -1) {         // if incoming byte is lower than 128 it is a data byte. Check if first data byte is empty (-1)
      val_byte1 = incoming;             // write first data byte (MSB)
    }
    else if (val_byte2 == -1) {         // check if second data byte is empty (-1)
      val_byte2 = incoming;             // write second data byte (LSB)
      float value = 128*val_byte1 + val_byte2;          // recombine the first and second data byte to the actual value that was sent from the phone
      // here is the location that you can implement the code what you want to do with the controller id and value received from the phone
      value =0;
      switch (id) {
          case 1:  
            feed();
            //value = readPH();
            Serial.print("Measured: "); Serial.print(value); Serial.print("\t");
            value = round(value * 100);
            Serial.print("Button 1 (PH): "); Serial.println(value);
            break;
          case 2:  
            //value = readTemp();
            Serial.print("Measured: "); Serial.print(value); Serial.print("\t");
            value *= 10;
            Serial.print("Button 2 (Temp): "); Serial.println(value);
            break;
          case 3: 
            //value = readLevel(); 
            Serial.print("Measured: "); Serial.print(value); Serial.print("\t");

            Serial.print("Button 3 (Level): "); Serial.println(value);
            break;
          default:
        
            break;
        }
      Serial.print(">Id: "); Serial.print(id); Serial.print(", val: "); Serial.println(value);   // for debugging write to the serial interface (check with serial monitor)
      send_BT(id, value);               // for test purposes we just send the data back to the phone
      //reset_rx_BT();                    // not strictly needed, but just in case erase all bytes (set to -1)

    }

  }
  
}

void reset_rx_BT() {                    // function to erase all bytes (set to -1)
  id = -1;
  val_byte1 = -1;
  val_byte2 = -1;
}
void send_BT(int id, int value) {       // function to write id and value to the bluetooth interface (and split value in MSB and LSB
  ESP_BT.write(128 + id);
  ESP_BT.write((value/128));       // MSB
  ESP_BT.write(value%128);              // LSB
}
