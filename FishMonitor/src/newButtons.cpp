#include "Button_Handler.h"

void IRAM_ATTR isrSW1(){
  //digitalWrite(SW1, !digitalRead(LED_pin));
}
void IRAM_ATTR isrSW2(){
  //digitalWrite(SW2, !digitalRead(LED_pin));
}
void IRAM_ATTR isrSW3(){
  //digitalWrite(SW3, !digitalRead(LED_pin));
} 
void setupButtons(){
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  attachInterrupt(SW1, isrSW1, FALLING);
  attachInterrupt(SW2, isrSW1, FALLING);
  attachInterrupt(SW3, isrSW1, FALLING);

}
