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

uint16_t Xx=10; // Should be 10
uint16_t Yy=60; // can be mare than 60
uint16_t Ww=300;
uint16_t Hh=20;
uint16_t  Ccolor=Black;
byte MinDays=1;
byte MaxDays=21;
void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); 
  tft.fillScreen(White);
  slider(Xx,Yy,Ww,Hh,Ccolor);
 
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
  //if(p.x > 187 && p.x <208 && p.y >10 && p.y< 215 )
  if(p.x > Xx+177 && p.x < Xx+198 && p.y >Yy-50 && p.y< Yy+155){
     //void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  //void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
   //days= map(p.y, 10 ,210 ,1,21);
   days= map(p.y, Yy-50 ,Yy+150 ,MinDays,MaxDays);
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

void slider(uint16_t x,uint16_t y, uint16_t w,uint16_t h,uint16_t color){
  //void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  //tft.drawRect( 10,60,300,10,Black);
  tft.drawRect( x,y,w,h,color);
  tft.setCursor(x+2,y-10);
  tft.setTextSize (1);
  tft.setTextColor(color);
  tft.println("1");
  tft.setCursor(x+290,y-10);
  tft.setTextSize (1);
  tft.setTextColor(color);
  tft.println("21");
  tft.setCursor(x+255,y+20);
  tft.setTextSize (1);
  tft.setTextColor(color);
  tft.print("Days: ");
}


