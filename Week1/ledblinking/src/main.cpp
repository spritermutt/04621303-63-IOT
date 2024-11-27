#include <Arduino.h>
const int LED_BUILT_IN_BLINK = 0;
const int LED_BLINK = 1;
int state;
void setup() {
  state = LED_BUILT_IN_BLINK;
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(16,OUTPUT);
}

void loop() {
  if (state == LED_BUILT_IN_BLINK)
  {
    for (int i = 0; i < 2; i++)
    {
      digitalWrite(LED_BUILTIN,LOW);
      delay(500);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(500);
    }
    state = LED_BLINK;   
  }
  if (state == LED_BLINK)
  {
    digitalWrite(16,HIGH);
    delay(500);
    digitalWrite(16,LOW);
    delay(500);
    state = LED_BUILT_IN_BLINK; 
  }
  
  
}


