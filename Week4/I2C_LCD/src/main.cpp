#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
const int LCDIDLE =0;
const int LCDLOOP = 1;
int state;
float var;
LiquidCrystal_I2C LCD(0x27,16,2);

void setup(){
  Serial.begin(115200);
  LCD.init();
  LCD.clear();
  LCD.backlight();
  state = LCDIDLE;
}
void loop(){
  if(state==LCDIDLE){
    var = analogRead(A0);
    var = var * (3.3 / 1023.0);  // Calculate voltage accurately    delay(500);
    state = LCDLOOP;
  }else if(state == LCDLOOP){
 LCD.setCursor(0,0);
    LCD.print(var,2);
    LCD.print("V");
    Serial.println(var);
    delay(1000);
    state = LCDIDLE;
  }
}