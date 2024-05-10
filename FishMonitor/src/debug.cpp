#include "debug.h"
#include "Sensor_Handler.h"
#include "Bluetooth_Handler.h"
#include "LCD_Handler.h"
extern int delayMinutes;
extern hw_timer_t * timer1;
extern hw_timer_t * timer3;

extern int targetHour, targetMinute;

void checkSerial(){
  char receivedCommand;
  if (Serial.available() > 0){ //if there's something in the serial port
      // store the value in the serial port to the receivedCommand variable
      receivedCommand = Serial.read();  
      // Only enter this long switch-case statement if there is a new command from the user
      switch (receivedCommand){ //we check what is the command
        case '3':
          Serial.println(timerAlarmReadSeconds(timer3));
          break;
        case '1':
          Serial.printf("Alarm Seconds: %02d\n", timerAlarmReadSeconds(timer1));
          Serial.printf("Timer Seconds: %02d\n", timerReadSeconds(timer1));
          break;
        case 'R':
          Serial.printf("Remaining minutes: %d", timerReadSeconds(timer3) / 60);
          break;
        default:  break;
      }
  }

}
