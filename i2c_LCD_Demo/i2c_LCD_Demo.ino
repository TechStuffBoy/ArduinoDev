#include<Wire.h>
#include<LCD.h>
#include<LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27

#define RS_PIN 4
#define RW_PIN 5
#define EN_PIN 6
#define DB4_PIN 9
#define DB5_PIN 10
#define DB6_PIN 11
#define DB7_PIN 12

#define BACKLIGHT 3

//LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

LiquidCrystal_I2C lcd(I2C_ADDR,EN_PIN,RW_PIN,RS_PIN,DB4_PIN,DB5_PIN,DB6_PIN,DB7_PIN,BACKLIGHT,POSITIVE);

void setup()
{
    lcd.begin(16,2);
    lcd.backlight();
    //lcd.setBacklightPin(BACKLIGHT,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
   
}

void loop()
{
   lcd.print("Hello,World!");
   delay(1000);  
}

