#include <Arduino.h>
const int Read_analog =0;
const int MOTOR_spin = 1;
int state,var;

void setup(){
  Serial.begin(115200);
  pinMode(D2,OUTPUT);
  pinMode(D1,OUTPUT);
  state = Read_analog;
}
void loop(){
  if(state==Read_analog){
    var = analogRead(A0);
    var = map(var,0,1023,0,255);
    Serial.print("Analog Value: ");
    Serial.println(var);
    delay(500);
    state = MOTOR_spin;
  }else if(state == MOTOR_spin){
    analogWrite(D1,0);
    analogWrite(D2,var);
    delay(1000);
    state = Read_analog;
  }
}