// Library For TFT Lcd
#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
                    /******TFT LCD STUFF *******/
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

//#define YP A3  // must be an analog pin, use "An" notation!
//#define XM A2  // must be an analog pin, use "An" notation!
//#define YM 9   // can be a digital pin
//#define XP 8   // can be a digital pin

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_MINX 125-55
#define TS_MINY 905 //85
#define TS_MAXX 965
#define TS_MAXY 85//905
/*
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
*/
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

//Colors 
#define Black           0x0000
#define Blue            0x001F
#define Green           0x07E0
#define Red             0xF800
#define Yellow          0xFFE0 
#define White           0xFFFF
#define Orange          0xFD20
#define Pink            0xF81F
#define MINPRESSURE 10
#define MAXPRESSURE 1000
// Instantiating TouchScreen and LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define BOXSIZE 40
#define PENRADIUS 3
/******TFT LCD STUFF ENDS *******/

void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tftDisplayNoCenterWithNumber("7",3,Black,0,0,true,White,0);
    tftDisplayNoCenterWithNumber("765",3,Black,40,40,false,White,0);
}
void loop()
{
    
}
void tftDisplayNoCenterWithNumber(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor,int delayTimeout)
{
    if( clearFlag) {
        tft.fillScreen(clearScreenColor);
    }
    byte msgLength = msg.length();
    byte maxLength = 6;
    int space =  maxLength - msgLength;
    tft.setCursor(x,y);
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    for(int i=0 ; i<= space ;i++){
        tft.print("0");
    }
    tft.print(msg);
    delay(delayTimeout);   
}
