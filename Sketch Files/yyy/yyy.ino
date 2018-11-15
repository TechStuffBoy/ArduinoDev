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
    tft.fillScreen(White);
    
    //tft.drawRoundRect(15,80,135,35,4,White);
    tft.fillRect(0,0,320,33,Black);
    tft.fillRect(0,32,155,33,0x07E0);
    
    tft.setCursor (85,10);
    tft.setTextSize (2);
    tft.setTextColor(White);
    tft.println("CPS GATEWAY");
    
    tft.setCursor (20,40);
    tft.setTextSize (2);
    tft.setTextColor(Black);
    tft.println("WorkCenter");
    
    tft.drawFastHLine(0, 62, 320, Black);
    tft.drawFastHLine(0, 63, 320, Black);
    
    tft.setCursor (20,70);
    tft.setTextSize (2);
    tft.setTextColor(Black);
    tft.println("WorkOrder");
    
    tft.drawFastHLine(0, 92, 320, Black);
    tft.drawFastHLine(0, 93, 320, Black);
    
    tft.drawFastVLine(155,32,60, Black);
    tft.drawFastVLine(156,32,60, Black);
    tft.setCursor (40,100);
    tft.setTextSize (2);
    tft.setTextColor(Black);
    tft.println("OPERATOR");
    
    tft.setCursor (135,95);
    tft.setTextSize (4);
    tft.setTextColor(Black);
    tft.println(":");

    tft.setCursor (25,125);
    tft.setTextSize (2);
    tft.setTextColor(Black);
    tft.println("RAW MATERIAL\n    RECIEVED");

    createCPSGatewayLogo(Yellow, 200, 270);

}

unsigned long createCPSGatewayLogo(uint16_t logoColor, int xLoc, int yLoc) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height();
  //tft.fillScreen(White);
  start = micros();
  int xD,yD;
  for(x=0; x<3; x+=1)
  {
    xD = xLoc + x;
    yD = yLoc + x;
    tft.drawLine(yD+15 , xD+15 , yD+20 , xD+10  , logoColor);
    tft.drawLine(yD+20 , xD+10 , yD+10 , xD     , logoColor);
    tft.drawLine(yD+10 , xD    , yD    , xD+10  , logoColor);
    tft.drawLine(yD    , xD+10 , yD+20 , xD+30  , logoColor);
    tft.drawLine(yD+20 , xD+30 , yD+10 , xD+40  , logoColor);
    tft.drawLine(yD+10 , xD+40 , yD    , xD+30  , logoColor);
    tft.drawLine(yD    , xD+30 , yD+5  , xD+25  , logoColor);
  } 
  return micros() - start;
}
