#include "Sensor_Handler.h"

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature tempSensor(&oneWire);

Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

extern volatile bool allowFeed; 
float data[4];


void setupSensors() {
  tempSensor.begin();

  pinMode(pH_PIN, INPUT);

  pinMode(SIGNAL_PIN,INPUT);  // Water level signal
  //pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  //digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  myStepper.setSpeed(10);
  data[FOOD_COUNT] = 14;
  motorOff();

}

void readSensors() {
  tempSensor.requestTemperatures(); 
  //float tempF = tempSensor.getTempFByIndex(0);
  //data[TEMP] = round(tempF*10) / 10;
  //digitalWrite(POWER_PIN,LOW);
  data[TEMP] = tempSensor.getTempFByIndex(0);
  data[TEMP] = ((int)round(data[TEMP]*10))/10; // truncating to nearest 0.1
  
    // Check if reading was successful
    //Serial.print("Temperature is: ");
    //Serial.println(tempSensor.getTempFByIndex(0));
    //Serial.println("°F");

  float voltage = analogRead(pH_PIN) * (VOLTAGE / 4095.0); // Convert to voltage
  //float pH = -5.6548 * voltage + 15.509 + OFFSET; // Convert voltage to pH (example equation, needs calibration)
  data[PH] = -6.1216 * voltage + 15.162; // new formula
 
  //digitalWrite(POWER_PIN, HIGH);   // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  data[WATER_LEVEL] = analogRead(SIGNAL_PIN); // read the analog value from sensor
  //digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  
  #ifndef DEBUG
    Serial.print("TEMP: ");  Serial.println(data[TEMP]);
    Serial.print("PH: ");   Serial.println(data[PH]);  
    Serial.print("LEVEL: "); Serial.println(data[WATER_LEVEL]);
    Serial.print("COUNT: "); Serial.println(data[FOOD_COUNT]);
  #endif
}

void feed() {
  // step one segment:
  if((data[FOOD_COUNT] > 0)){
    Serial.print("Feeding... #"); Serial.println(data[FOOD_COUNT]);
    myStepper.step(ONE_SERVING);
    data[FOOD_COUNT]--;
    allowFeed = false;
    motorOff();
    delayMicroseconds(10);
  }
  else{
    Serial.println("Please refill Food");
  }
}

void autoFeed() {
  // step one segment:
  if((data[FOOD_COUNT] > 0)){
    Serial.print("Feeding... #"); Serial.println(data[FOOD_COUNT]);
    myStepper.step(ONE_SERVING);
    data[FOOD_COUNT]--;
    allowFeed = false;
    motorOff();
    delayMicroseconds(10);
  }
  else{
    Serial.println("Please refill Food");
  }
}

void motorOff(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
