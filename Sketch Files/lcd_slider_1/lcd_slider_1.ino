#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
int days=01;
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif
#define YP A3  
#define XM A2  
#define YM 9   
#define XP 8   
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
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
#define BOXSIZE 40
#define PENRADIUS 3
#define MINPRESSURE 10
#define MAXPRESSURE 1000
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
uint16_t Xx=10; // Should be 10
uint16_t Yy=150; // can be mare than 60
uint16_t Ww=300;
uint16_t Hh=10;
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

 
 }

void loop(){
  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //tft.fillRect(Xx+10,Yy ,5,10,Red );
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
//  Serial.print("("); Serial.print(p.x);
//  Serial.print(", "); Serial.print(p.y);
//  Serial.println(")");

 
 if(p.x > Xx+60 && p.x < Xx+105 && p.y >Yy-135 && p.y< Yy+65){
  int Y=p.y;
  Serial.println(Y);
  tft.fillRect(Xx+10,Yy ,5,10,Black );
  tft.fillRect(Y,Yy ,5,10,Red );
  tft.fillRect( Xx,Yy,Ww,Hh,Black);
  tft.fillRect(Y,Yy ,5,10,Red );
 }

if(p.x > Xx+60 && p.x < Xx+105 && p.y >Yy-135 && p.y< Yy+65){
  days= map(p.y, Yy-135 ,Yy+55 ,1,21); //Yy=150
   //Serial.print("Day:");
   //Serial.println(days);
        if(days<10){
        tft.setCursor(Xx+268,Yy+25);
        tft.setTextSize(2);
        tft.setTextColor(Red,White);
        tft.print(" ");
        tft.print(days);
       }  
        if(days < (MaxDays+1) && days>9){
        tft.setCursor(Xx+268,Yy+25);
        tft.setTextSize (2);
        tft.setTextColor(Red,White);
        tft.print(days);
       }
    }
  }
}

void slider(uint16_t x,uint16_t y, uint16_t w,uint16_t h,uint16_t color){
  //void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  //tft.drawRect( 10,60,300,10,Black);
  tft.fillRect( x,y,w,h,color);
  tft.setCursor(x-5,y-10);// 5,140
  tft.setTextSize(1);
  tft.setTextColor(Red);
  tft.println("1"); 
  tft.setCursor(x+290,y-10);
  tft.setTextSize (1);
  tft.setTextColor(Red);
  tft.println("21");
  tft.setCursor(x+210,y+25);
  tft.setTextSize (2);
  tft.setTextColor(Red);
  tft.print("DAYS: ");
  tft.print(days);
//  positioner();
}

//void positioner(){
//  TSPoint p=ts.getPoint();
//  pinMode(XM, OUTPUT);
//  pinMode(YP, OUTPUT);
//  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
//  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
//  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
//  Serial.print("("); Serial.print(p.x);
//  Serial.print(", "); Serial.print(p.y);
//  Serial.println(")");
//
// if( p.y >Yy-135 && p.y< Yy+65 ){
//   int X=p.x;
//   tft.fillRect( p.y,X ,5,20,Red );
// }
//}
//}
