#include<Wire.h>
#include "rgb_lcd.h"
int buttn= 3;
int potValue=0;
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
  Serial.begin(9600);

  
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("Pot value=");

  lcd.setCursor(3,1);
  lcd.print("SWITCH:");
  }
 void loop()
  {
  Potentiometer();
  button();
  delay(1000);
  }

  void Potentiometer()
  {
  
  potValue=analogRead(0);
  potValue=map(potValue,0,1023,0,255);
  Serial.println(potValue);
  lcd.setCursor(11,0);
  lcd.print(potValue);
  delay(10);
  lcd.print("   ");
  }

   void button()
  {
  
    buttonState= digitalRead(buttn);

   if(buttonState == HIGH)
   {
    digitalWrite(indicator, HIGH );
    lcd.setCursor(11,1);
    lcd.print("ON");
    delay(10);
    lcd.print("  ");
    }

   if(buttonState== LOW )
   {
    digitalWrite(indicator, LOW );
    lcd.setCursor(11,1);
    lcd.print("OFF");
    delay(10);
    lcd.print("  ");
    }
}


  
