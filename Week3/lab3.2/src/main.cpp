#include <Arduino.h>
int PWMstate=0;
int READ_ANALOG=0;
void setup() {
  Serial.begin(115200);
  PWMstate = READ_ANALOG;
}

void loop(){
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (3.3 / 1023.0);
  int PWM = map(sensorValue, 0, 1023, 0, 255);  
  if(PWMstate==READ_ANALOG){
  Serial.print("A0 = ");
  Serial.print(sensorValue);
  Serial.print("\tPWM = ");
  Serial.print(PWM);
  Serial.print("\tVoltage = ");
  Serial.println(voltage, 3);}
}
