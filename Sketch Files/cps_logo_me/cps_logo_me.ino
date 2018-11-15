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

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Pink            0xF81F

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

//  tft.drawLine(75,150,220,75,Black);
//  tft.drawLine(220,75,240,125,Black);
//  tft.drawLine(240,125,200,145,Black);
//  tft.drawLine(200,145,190,125,Black);
//
//
//   tft.drawLine(75,150,45,105,Black);
//  tft.drawLine(45,105,85,75,Black);
//  tft.drawLine(85,75,105,100,Black);
for(int i=0;i<5;i++){
 tft.drawLine(75+i,150+i,220+i,75+i,Black);
  tft.drawLine(220+i,75+i,240+i,125+i,Black);
  tft.drawLine(240+i,125+i,200+i,145+i,Black);
  tft.drawLine(200+i,145+i,190+i,125+i,Black);


   tft.drawLine(75+i,150+i,45+i,105+i,Black);
  tft.drawLine(45+i,105+i,85+i,75+i,Black);
  tft.drawLine(85+i,75+i,105+i,100+i,Black);

}

  
 //cps_logo(80,80);
// tft.drawPixel(300,230,Black);
// tft.drawPixel(68,-40,Black);
// tft.drawPixel(50,5,Black);

}

 void cps_logo( int x ,int y ){
//  tft.drawLine( x,y,x+20,y+20,Black);
//  tft.drawLine( x+20,y+20,x+30,y+5,Black);
//  tft.drawLine( x,y,x-20,y-20,Black);
//  tft.drawLine( x-20,y-20,x+80,y+40,Black);
//  tft.drawLine( x+80,y+40,x+90,y+35,Black);
//  tft.drawLine( x+90,y+35,x+80,y-5,Black);
//  tft.drawLine( x+80,y-5,x+65,y+5,Black);

 tft.drawLine( x,y,x+60,y-60,Black);
 tft.drawLine(x+60,y-60,x-35,y+15,Black);
//tft.drawLine(x+60,y-60,x+35,y-75,Black);
//tft.drawLine(x+35,y-75,x+35,y-65,Black);
//tft.drawLine(x+35,y-65,x+40,y-55,Black);
 }

  void loop(){
    
  }

