#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h> // F Malpartida's NewLiquidCrystal library
#define I2C_ADDR 0x27 // Define I2C Address for controller
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
#define BACKLIGHT 3

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void setup()
{
lcd.begin (20,4); // initialize the lcd

// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT,POSITIVE);
lcd.setBacklight(HIGH);

// Reset the display
lcd.clear();
delay(1000);
lcd.home();

// Print on the LCD
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Hello,Arduino!");
delay(1000);
}

void loop()
{

/*
 // Reset the display
lcd.clear();
delay(1000);
lcd.home();

// Print on the LCD
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("Hello,Arduino!");
delay(1000);
 */
}


