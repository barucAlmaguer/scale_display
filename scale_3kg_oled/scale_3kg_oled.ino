#include "hx711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Hx711.DOUT - pin #A1
// Hx711.SCK - pin #A0
Hx711 scale(A1, A0);
byte times = 1;
int gramos_cal;

void setup()
{
  Serial.begin(9600);
  //Serial.println("hola");
  //Scale, Offset:0.001199529647,-9740.830078125000
  scale.setReadTimes(1);
  scale.setScale(0.001199529647f);
  scale.setOffset(-9741.43007812f);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Scale");
  display.println("Calibrating...");
  gramos_cal = scale.getGram();
  display.display();
  delay(1000);
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
}

void loop()
{
  int gramos = scale.getGram() - gramos_cal;
  Serial.println(gramos);
  
//  int val = scale.averageValue();
//  Serial.println(gramos);
//  display.clearDisplay();
//  display.setCursor(0, 7);
//  display.print(val);
//  //display.print(".");
//  display.display();

  display.clearDisplay();
  display.setCursor(0, 7);
  display.print(gramos);
  display.print(" g");
  display.display();
  delay(100);
}


