#include "Arduino.h"

const int LED_ON = 0;
const int LED_OFF = 1;
int state;
int lastButtonState = LOW; 
int buttonState; 

void setup() {
  pinMode(D0, INPUT);
  pinMode(D1, OUTPUT); 
  state = LED_OFF; 
  Serial.begin(9600); 
}

void loop() {
  buttonState = digitalRead(D0);
  Serial.println(lastButtonState);

  switch (state) {
    case LED_OFF:
      if (buttonState == LOW && lastButtonState == HIGH) {
        state = LED_ON;
        Serial.println("LED ON");
        delay(200);
      }
      digitalWrite(D1, LOW);
      break;

    case LED_ON:
      if (buttonState == LOW && lastButtonState == HIGH) {
        state = LED_OFF;
        Serial.println("LED OFF");
        delay(200); 
      }
      digitalWrite(D1, HIGH); 
      break;
  }
  lastButtonState = buttonState;
}