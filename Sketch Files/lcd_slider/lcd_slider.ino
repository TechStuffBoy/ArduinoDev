#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
int days=1;
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define Black           0x0000 
#define White           0xFFFF 
#define Red             0xF800
#define Yellow          0xFFE0
#define Green           0x07E0

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define BOXSIZE 40
#define PENRADIUS 3


void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); 

  tft.fillScreen(White);
 
  //void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  tft.drawRect( 10,60,300,10,Black);
  tft.setCursor(12,50);
  tft.setTextSize (1);
  tft.setTextColor(Black);
  tft.println("1");
  tft.setCursor(300,50);
  tft.setTextSize (1);
  tft.setTextColor(Black);
  tft.println("21");
  tft.setCursor(265,80);
  tft.setTextSize (1);
  tft.setTextColor(Black);
  tft.print("Days: ");
 // tft.print(days);
  
  
}
#define MINPRESSURE 10
#define MAXPRESSURE 1000
void loop(){
  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  if(p.x > 187 && p.x <208 && p.y >10 && p.y< 215 ){
     //void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  //void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
   
   days= map(p.y, 10 ,210 ,1,21);
   Serial.print("Day:");
   Serial.println(days);
        if(days<10) {
        tft.setCursor(300,80);
        tft.setTextSize (1);
        tft.setTextColor(Red,White);
        tft.print("0");
        tft.print(days);
       }  
        if(days<22 && days>9){
        tft.setCursor(300,80);
        tft.setTextSize (1);
        tft.setTextColor(Red,White);
        tft.print(days);
       }
    }
  }
}
