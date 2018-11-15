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

int touchAreaIndex = 0;
int len = 0;
String touchStr;

struct touchAreaSt {
    int x1;
    int x2;
    int y1;
    int y2;
    String str;
};

struct touchAreaSt touchArea[10];
 
void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
} 
void loop()
{
    homeScreen();
    while(buttonPressed() == false);
    tft.fillScreen(Black);
    boolean val = buttonPressed();
    Serial.println(val);
    if(buttonPressed() == true) {
        Serial.println("Button Pressed.  ret String = " + touchStr);
    }
   delay(10);
}



void homeScreen()
{
    clearTouchButtons();
    tft.fillScreen(White);
    createTouchButton("PREV",20,30,2,Black,Green,Red);
    createTouchButton("SELECT",120,35,2,Black,Green,Red);
    createTouchButton("NEXT",220,30,2,Black,Green,Red);

    createTouchButton("PREV",50,70,2,Black,Green,Red);
    createTouchButton("SELECT",50,120,2,Black,Green,Red);
    createTouchButton("NEXT",50,170,2,Black,Green,Red);
}

void clearTouchButtons()
{
    for(int i=0; i < sizeof(touchArea)/sizeof(touchAreaSt); i++) {
        touchArea[i].x1 = 0;
        touchArea[i].y1 = 0;
        touchArea[i].x2 = 0;
        touchArea[i].y2 = 0;
        touchArea[i].str = "";
    }
    touchAreaIndex = 0;
}


void createTouchButton(String msg,int cursorX,int cursorY,byte textSize,int textColor,int rectColor,int buttonSelectColor)
{
        if(textSize == 0) {
            textSize = 1;
        }
        
        int rectRadius = textSize;
        //static int len;
        len = msg.length();
        int sizeXFactor[] = {0,6,12,18,24,32}; //320/(no of char for textSize)
        int xMargin[] = {0,1,6,6,6,6};
        int sizeYFactor[] = {0,8,16,24,34,40}; //240/(no of lines for textSize)
        int yMargin[] = {0,1,6,6,6,6};
        int sizeX = len * sizeXFactor[textSize];
        int sizeY = sizeYFactor[textSize] ;
        int left = cursorX - xMargin[textSize];
        int top = cursorY - yMargin[textSize];
        int horiSize = (sizeX + 2*xMargin[textSize]);
        int vertSize = (sizeY + 2*yMargin[textSize]);
        touchArea[touchAreaIndex].x1 = left;
        touchArea[touchAreaIndex].y1 = top - 2*len;
        touchArea[touchAreaIndex].x2 = left + horiSize;
        touchArea[touchAreaIndex].y2 = top + vertSize - len;
        touchArea[touchAreaIndex].str = msg;
        Serial.print("sizeX :");
        Serial.println(sizeX);
        Serial.print("sizeY :");
        Serial.println(sizeY);
        Serial.print("X1 :");
        Serial.println(touchArea[touchAreaIndex].x1);
        Serial.print("X2 :");
        Serial.println(touchArea[touchAreaIndex].x2);
        Serial.print("Y1 :");
        Serial.println(touchArea[touchAreaIndex].y1);
        Serial.print("Y2 :");
        Serial.println(touchArea[touchAreaIndex].y2);
        Serial.print("HorizontalSize :");
        Serial.println(horiSize);
        Serial.print("VerticalSize :");
        Serial.println(vertSize);
        Serial.print("Left:");
        Serial.println(left);
        Serial.print("Top:");
        Serial.println(top);

        Serial.println("<------------------------------------------->");
        
        Serial.print("RectX :");
        Serial.println((touchArea[touchAreaIndex].x1));
        Serial.print("RectY :");
        Serial.println((touchArea[touchAreaIndex].y1));
        Serial.print("Width :");
        Serial.println((touchArea[touchAreaIndex].x2-touchArea[touchAreaIndex].x1));
        Serial.print("Height :");
        Serial.println((touchArea[touchAreaIndex].y2-touchArea[touchAreaIndex].y1));
        Serial.print("P.Xmin :");
        Serial.println(touchArea[touchAreaIndex].x1);
        Serial.print("P.Xmax :");
        Serial.println((2*(touchArea[touchAreaIndex].x2) -  2*(touchArea[touchAreaIndex].x1)));
        Serial.print("P.Ymin :");
        Serial.println(touchArea[touchAreaIndex].y1);
        Serial.print("P.ymax :");
        Serial.println((touchArea[touchAreaIndex].y2 -  touchArea[touchAreaIndex].y1));

        Serial.println("<------------------------------------------->");
        //String str =  "Button In createTouchButton X1= " + String(touchArea[touchAreaIndex].x1) + " Y1 = " +  String(touchArea[touchAreaIndex].y1) + " x2 = " +  String(touchArea[touchAreaIndex].x2) + " Y2 = " + String(touchArea[touchAreaIndex].y2) + " Msg = " + msg ;
        //Serial.println(str);
        
        //void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
        //tft.fillRoundRect(left, top, horiSize, vertSize, rectRadius, rectColor);
        tft.fillRoundRect(touchArea[touchAreaIndex].x1, touchArea[touchAreaIndex].y1, (touchArea[touchAreaIndex].x2-touchArea[touchAreaIndex].x1), (touchArea[touchAreaIndex].y2 - touchArea[touchAreaIndex].y1), rectRadius, rectColor);
        //tft.fillRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, rectRadius, rectColor);
        tft.setCursor((cursorX ), (cursorY- len));
        tft.setTextSize(textSize);
        tft.setTextColor( textColor );
        tft.print(msg);
        touchAreaIndex++;
}

boolean buttonPressed() {
    if(touchAreaIndex == 0)
        return false;
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
        p.y = map(p.y, TS_MINY-40, TS_MAXY, 0, tft.height());
        Serial.print("("); Serial.print(p.x);
        Serial.print(", "); Serial.print(p.y);
        Serial.println(")");

        for(int i=0; i < sizeof(touchArea)/sizeof(touchAreaSt); i++) {
             if (( (p.x >= touchArea[i].x1 - len)  && p.x <= (touchArea[i].x2 -  touchArea[i].x1) && p.y >= touchArea[i].y1 && p.y <= (touchArea[i].y2 - touchArea[i].y1))) {
                touchStr = touchArea[i].str;
                tft.drawRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 3, Red);
                tft.drawRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 3, Red);
                //tft.drawRoundRect(touchArea[i].x1+1, touchArea[i].y1+1, (touchArea[i].x2-touchArea[i].x1)+1, touchArea[i].y2+1 - touchArea[i].y1+1, 3, Red);
                clearTouchButtons();
                return(true);
             }
        }
    }
    return false;
}

