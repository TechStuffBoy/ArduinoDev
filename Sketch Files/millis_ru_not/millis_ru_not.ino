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

boolean flag = false;
boolean printLetter =true;
unsigned long previousMillis = 0;
int interval = 8000;

const char* text = "RU is Not Configured";

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
  tft.fillScreen(White);
  Serial.println(text);
}
void loop(){

  if(Serial.available() ){
    char c = Serial.read();
    if( c == 'a' ){
      flag = true;
    }
  }
 
  if(flag == true ){
      if( printLetter == true ) {
        //Serial.println( " RU is Not Configured ");
        tft.setCursor (0,100);
        tft.setTextSize (2);
        tft.setTextColor(Black);
        center( text , 2);
      }
      unsigned long currentMillis = millis();
      while(currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          printLetter = false;
          tft.fillScreen(White);
          text="";
          flag = false;
          }
   }
}


void center( const char* text, byte textSize )
{
// if textSize(1)--> max char 53
// if textSize(2)--> max char 26
// if textSize(3)--> max char 17
// if textSize(4)--> max char 13
// if textSize(5)--> max char 10
  tft.setTextSize(textSize);
  String words=text;
  char str[50];
  int len;
  
  strcpy(str, text);
  len = strlen(str);
  if( textSize == 1 ){
    int x=0;
    int space= 53;
    x= ( space / 2 ) - ( len / 2 );
    //Serial.println(x);
    for(int i=0; i<=x ;i++){
      tft.print(" ");
    }
    tft.println(str);
   }

    if( textSize == 2 ){
      int x=0;
      int space= 24;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 3 ){
      int x=0;
      int space= 17;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 4 ){
      int x=0;
      int space= 13;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }

    if( textSize == 5 ){
      int x=0;
      int space= 10;
      x= ( space / 2 ) - ( len / 2 );
      //Serial.println(x);
      for(int i=0; i<=x ;i++){
        tft.print(" ");
      }
      tft.println(str);
    }
    
    memset(str,0,50);
}



