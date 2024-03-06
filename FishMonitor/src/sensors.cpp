#include "Sensor_Handler.h"

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature tempSensor(&oneWire);
int value = 0;


void setupSensors() {
  tempSensor.begin();

  pinMode(pH_PIN, INPUT);

  pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF
}

void readSensors() {
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  
  tempSensor.requestTemperatures(); 
  float tempF = tempSensor.getTempFByIndex(0);
  tempF = round(tempF*10) / 10;
    // Check if reading was successful
    //Serial.print("Temperature is: ");
    //Serial.println(tempSensor.getTempFByIndex(0));
    //Serial.println("°F");

  float voltage = analogRead(pH_PIN) * (VOLTAGE / 4095.0); // Convert to voltage
  //float pH = -5.6548 * voltage + 15.509 + OFFSET; // Convert voltage to pH (example equation, needs calibration)
  float pH = -6.1216 * voltage + 15.162; // new formula
  //Serial.println(voltage);
  //Serial.print("The pH is: ");
  //Serial.println(pH);

  delay(10);                      // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  //Serial.print("The water sensor value: ");
  Serial.println(value);
}