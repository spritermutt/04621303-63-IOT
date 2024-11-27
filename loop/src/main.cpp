#include <Arduino.h>

// Define states for each digit of the number
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
#define STATE_4 4
#define STATE_5 5
#define STATE_6 6
#define STATE_7 7
#define STATE_8 8
#define STATE_9 9
#define STATE_10 10
#define STATE_11 11
#define STATE_12 12
#define STATE_13 13
#define STATE_14 14

int currentState = STATE_1;  // Start from state 1 (first digit)

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
}

void loop() {
  if (currentState == STATE_1) {
    Serial.println("1");  
    currentState = STATE_2;  
    delay(1000);  
  }
  else if (currentState == STATE_2) {
    Serial.println("1");  
    currentState = STATE_3;  
    delay(1000);  
  }
  else if (currentState == STATE_3) {
    Serial.println("6");  
    currentState = STATE_4;  
    delay(1000);  
  }
  else if (currentState == STATE_4) {
    Serial.println("6");  
    currentState = STATE_5;  
    delay(1000);  
  }
  else if (currentState == STATE_5) {
    Serial.println("3");  
    currentState = STATE_6;  
    delay(1000);  
  }
  else if (currentState == STATE_6) {
    Serial.println("0");  
    currentState = STATE_7;  
    delay(1000);  
  }
  else if (currentState == STATE_7) {
    Serial.println("4");  
    currentState = STATE_8;  
    delay(1000);  
  }
  else if (currentState == STATE_8) {
    Serial.println("6");  
    currentState = STATE_9;  
    delay(1000);  
  }
  else if (currentState == STATE_9) {
    Serial.println("2");  
    currentState = STATE_10;  
    delay(1000);  
  }
  else if (currentState == STATE_10) {
    Serial.println("0");  
    currentState = STATE_11;  
    delay(1000);  
  }
  else if (currentState == STATE_11) {
    Serial.println("0");  
    currentState = STATE_12;  
    delay(1000);  
  }
  else if (currentState == STATE_12) {
    Serial.println("1");  
    currentState = STATE_13;  
    delay(1000);  
  }
  else if (currentState == STATE_13) {
    Serial.println("-");  
    currentState = STATE_14;  
    delay(1000);  
  }
  else if (currentState == STATE_14) {
    Serial.println("4");  
    currentState = STATE_1;  
    delay(1000);  
  }
}
