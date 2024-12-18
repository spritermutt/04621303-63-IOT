#include <Arduino.h> 
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_SHTC3.h"
#include "LiquidCrystal_I2C.h"

const int LCDIDLE =0;
const int LCDLOOP = 1;
int state;
float var;
sensors_event_t humidity, temp; // Declare humidity and temp here to make them accessible in both states
LiquidCrystal_I2C LCD(0x27,16,2);
Adafruit_SHTC3 sht3 = Adafruit_SHTC3();

void setup(){
  Serial.begin(115200);
  LCD.init();
  LCD.clear();
  LCD.backlight();
  sht3.begin();
  state = LCDLOOP;
}

void loop(){
  if(state==LCDLOOP){
    sht3.getEvent(&humidity, &temp);
 LCD.setCursor(0,0);
    LCD.print("Humidity: ");
    LCD.print(humidity.relative_humidity, 2);
 LCD.setCursor(0,1);
    LCD.print("Temp: ");
    LCD.print(temp.temperature, 2);
    delay(1000);
  }
}