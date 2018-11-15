#include<Wire.h>
#include "rgb_lcd.h"
int buttn= 3;
int buttonState=0;
int indicator=13;



rgb_lcd lcd;

const int colorR= 128;
const int colorG=125;
const int colorB=33;

void setup()
{
  pinMode(buttn, INPUT);
  pinMode(indicator , OUTPUT);

  
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Hello, World ");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("This is a Analog");
  lcd.setCursor(0,1);
  lcd.print("Sensor Meter");
  delay(3000);
  lcd.clear();
  }
  void loop()
  {
  Potentiometer();
  button();
  }
  
  void Potentiometer()
  {
  int potValue;
  potValue=analogRead(0);
  potValue=map(potValue,0,1023,0,255);
  lcd.setCursor(0,0);
  lcd.print(" Pot value = ");
  lcd.print(potValue);
  delay(1500);
  lcd.clear();
  }
void button()
{
  
 buttonState= digitalRead(buttn);

   if(buttonState == HIGH)
   {
    digitalWrite(indicator, HIGH );
    lcd.setCursor(0,0);
    lcd.print( " SWITCH ON ");
    lcd.setCursor(0,1);
    lcd.print("See pin 13 led");
    
    delay(1500);
   }

   if(buttonState== LOW )
   {
    digitalWrite(indicator, LOW );
    lcd.setCursor(0,0);
    lcd.print(" SWITCH OFF ");
    lcd.setCursor(0,1);
    lcd.print("pin 13 is OFF");
    delay(1500);
   }
  lcd.clear();
}

