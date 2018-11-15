#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

char n[7] = {'N','a','n','d','h','u'};

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // turn on the cursor:
    lcd.cursor();
}

void loop() 
{
    for(int i=0;i<6;i++)
    {
      lcd.print(n[i]);
      if(n[i]== 'N')
      {
       lcd.rightToLeft(); 
      }
      if(n[i] == 'u')
      {
        lcd.leftToRight();
      }
      delay(1000);
    }
    lcd.clear();
}
