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
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
  drawBorder();

  tft.fillScreen(Black);

  
  
  //TFT has 320x240 pixel 
  tft.setCursor (65,10);
  tft.setTextSize (3);
  tft.setTextColor(White);
  tft.println("CPSGATEWAY");

  tft.setCursor (30,35);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.println("Technologies Pvt Ltd");
 // tft.drawLine(60, 60, 80,80, Red);

  
 
 
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop(){
  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
//    Serial.print("X = "); Serial.print(p.x);
//    Serial.print("\tY = "); Serial.print(p.y);
//    Serial.print("\tPressure = "); Serial.println(p.z);
//  
//
//  Serial.println();
//  Serial.println();
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  if( p.x > 150 && p.x< 200 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder1 ");
  }

   if( p.x > 85 && p.x< 145 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder2 ");
  }
  if( p.x > 20 && p.x< 70 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder3 ");
}
}
}

void drawBorder () {

  // Draw a border

  uint16_t width = tft.width() - 1;
  Serial.print("Width:");
  Serial.println(tft.width());
  uint16_t height = tft.height() - 1;
  uint8_t border = 5;

  tft.fillScreen(Blue);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), White);
  
}



