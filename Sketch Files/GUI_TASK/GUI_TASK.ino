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

#define Black           0x0000      /*   0,   0,   0 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Purple          0xC838      /* 128,   0, 128 */
#define Blue            0x8F7D      /* 0,   0, 255 */
#define Green           0x07E0      /* 0, 255,   157 */
#define LightGreen      0x8FB3      /*139, 246,   0 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define Red             0xF800      /* 255,   0,   0 */
#define LightRed        0xFDF9      /* 250,   191, 202 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
#define Pink            0xF81F
#define Violet          0x108B 
#define Rose         0xF87A
#define LightYellow     0xFFF8      /* 64, 100,  88 */
#define Beigh    0xF5B1      /* 245, 183,  141 */
#define Blue1   0x0C91      
#define Orange   0xF9A0 
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3

int CurrentPage=0;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
  HomeScreen();
  }

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop(){
  if(CurrentPage == 0){
  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE){
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
  }
 }
}
  void HomeScreen(){
CurrentPage=0; 
tft.fillScreen(Black);
tft.drawRect( 5,5,308,230,White);

//tft.drawRoundRect(13,15,135,35,4,Black);
tft.fillRoundRect(12,17,134,35,3,Orange);

tft.setCursor (25,25);
tft.setTextSize (2);
tft.setTextColor(White);
tft.print("WORKOrder");

tft.setCursor (145,10);
tft.setTextSize (7);
tft.setTextColor(White);
tft.println(":");

tft.fillRoundRect(176,17,134,35,3,Yellow);

tft.fillRoundRect(12,65,134,35,3,Orange);

tft.setCursor (25,75);
tft.setTextSize (2);
tft.setTextColor(White);
tft.print("WORKCenter");

tft.setCursor (145,10);
tft.setTextSize (7);
tft.setTextColor(White);
tft.println(":");

tft.fillRoundRect(176,17,134,35,3,Blue);

tft.setCursor (145,55);
tft.setTextSize (7);
tft.setTextColor(White);
tft.println(":");


tft.fillRoundRect(176,67,134,35,3,Orange);

tft.fillRoundRect(25,115,250,35,3,Orange);

tft.setCursor (40,125);
tft.setTextSize (2);
tft.setTextColor(White);
tft.print("OPERATOR");
tft.setCursor (140,105);
tft.setTextSize(7);
tft.print(":");

tft.setCursor (40,155);
tft.setTextSize (2);
tft.setTextColor(White);
tft.print("RM Check");

}

