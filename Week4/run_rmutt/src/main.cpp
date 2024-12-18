#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
String text = "RMUTT";
const int LCD_BEGIN = 0;
const int LCD_SCORING = 1;
int state = LCD_BEGIN;
int positions[5];
int rows[5];
int maxPos = 16;

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  if (state == LCD_BEGIN) {
    for (int i = 0; i < text.length(); i++) {
      positions[i] = i;
      rows[i] = 0;
    }
    state = LCD_SCORING;
  } else if (state == LCD_SCORING) {
    lcd.clear();
    for (int i = 0; i < text.length(); i++) {
      lcd.setCursor(positions[i], rows[i]);
      lcd.print(text[i]);
    }
    delay(300);
    for (int i = 0; i < text.length(); i++) {
      positions[i]++;
      if (positions[i] >= maxPos) {
        positions[i] = 0;
        rows[i] = (rows[i] + 1) % 2;
      }
    }
  }
}
