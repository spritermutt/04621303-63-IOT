#include <Arduino.h>

const int LED_1_ON = 0;
const int LED_2_ON = 1;
const int LED_3_ON = 2;
const int LED_4_ON = 3;

int state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  state = LED_1_ON;
}

void loop() {
  // Read and map the analog value
  int val = map(analogRead(A0), 0, 1023, 0, 100);
  Serial.print("Analog Value: ");
  Serial.println(val);

  if (val >= 0 && val < 25) {
    state = LED_1_ON;
  }
  else if (val >= 25 && val < 50) {
    state = LED_2_ON;
  }
  else if (val >= 50 && val < 75) {
    state = LED_3_ON;
  }
  else if (val >= 75) {
    state = LED_4_ON;
  }

  switch (state) {

    case LED_1_ON:
      digitalWrite(D5, HIGH);
      delay(1000);
      digitalWrite(D5, LOW);
      delay(1000);
      break;

    case LED_2_ON:
      digitalWrite(D5, HIGH);
      digitalWrite(D6, LOW);
      digitalWrite(D7, LOW);
      break;

    case LED_3_ON:
      digitalWrite(D5, HIGH);
      digitalWrite(D6, HIGH);
      digitalWrite(D7, LOW);
      break;

    case LED_4_ON:
      digitalWrite(D5, HIGH);
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
      break;

    default:
      break;
  }

  // Small delay to make sure the analog reading isn't updated too quickly
  delay(200);
}
