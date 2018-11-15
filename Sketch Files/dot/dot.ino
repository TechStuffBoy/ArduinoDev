#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
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

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BOXSIZE 40
#define PENRADIUS 3

void setup(){
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1);
  
  //Screen1();
//  initialize();
  
}

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
}
}
//void Screen1(){
//  tft.fillScreen(White);
//  tft.setCursor (105,10);
//  tft.setTextSize (2);
//  tft.setTextColor(Black);
//  tft.println("WELCOME TO");
//  tft.setCursor (50,40);
//  tft.println("MURUDESWAR CERAMICS");
//  tft.setCursor (90,70);
//  tft.println("PRIVATE LTD");
//  tft.setCursor (25,110);
//  tft.print("PLANT  ");
//  tft.setCursor(100,105);
//  tft.setTextSize(4);
//  tft.print("  :");
//  tft.setCursor(170,110);
//  tft.setTextSize(2);
//  tft.println("TUMKUR");
//  tft.setCursor (25,140);
//  tft.print("WORKCENTER");
//  tft.setCursor(145,135);
//  tft.setTextSize(4);
//  tft.print(":");
//  tft.setTextSize(2);
//  tft.setCursor(170,140);
//  tft.println("KILN1");
//  tft.setCursor(10,190);
//  tft.println("<---  SWIPE YOUR ID  --->");
//  }
void dot(int x,int y,byte size, int color){
  int i;
  tft.fillScreen(White);
//  tft.setCursor (40,140);
//  tft.setTextSize (2);
//  tft.setTextColor(Black);
//  tft.print("INITIALIZING");
  while(1){
  for(i=0;i<3;i++){
    if(i==0){
     tft.setCursor (x,y);
     tft.setTextColor(Black);
     tft.print(".");
     tft.setCursor (x+10,y);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (x+20,y);
     tft.setTextColor(White);
     tft.print(".");
     
    }
    if(i==1){
     tft.setCursor (x,y);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (x+10,y);
     tft.setTextColor(Black);
     tft.print(".");
     tft.setCursor (x+20,y);
     tft.setTextColor(White);
     tft.print(".");
     
    }
    if(i==2){
      tft.setCursor (x,y);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (110,140);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (120,140);
     tft.setTextColor(Black);
     tft.print(".");
    
     }
     
     if(i== 3){
        i=0;
        tft.setCursor (100,140);
        tft.setTextColor(Black,White);
        tft.print(".");
        tft.setTextColor(Black,White);
        tft.print(".");
        tft.setTextColor(Black,White);
        tft.print(".");
        
    }
    delay(1000);
   }
  }
}





/*****
 void dot(int x,int y,byte size, int color){
  int i;
  tft.fillScreen(White);
//  tft.setCursor (40,140);
//  tft.setTextSize (2);
//  tft.setTextColor(Black);
//  tft.print("INITIALIZING");
  while(1){
  for(i=0;i<3;i++){
    if(i==0){
     tft.setCursor (100,140);
     tft.setTextColor(Black);
     tft.print(".");
     tft.setCursor (110,140);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (120,140);
     tft.setTextColor(White);
     tft.print(".");
     
    }
    if(i==1){
     tft.setCursor (100,140);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (110,140);
     tft.setTextColor(Black);
     tft.print(".");
     tft.setCursor (120,140);
     tft.setTextColor(White);
     tft.print(".");
     
    }
    if(i==2){
      tft.setCursor (100,140);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (110,140);
     tft.setTextColor(White);
     tft.print(".");
     tft.setCursor (120,140);
     tft.setTextColor(Black);
     tft.print(".");
    
     }
     
     if(i== 3){
        i=0;
        tft.setCursor (100,140);
        tft.setTextColor(Black,White);
        tft.print(".");
        tft.setTextColor(Black,White);
        tft.print(".");
        tft.setTextColor(Black,White);
        tft.print(".");
        
    }
    delay(1000);
   }
  }
}
 */

