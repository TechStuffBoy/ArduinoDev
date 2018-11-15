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
int i=0;
/******TFT LCD STUFF ENDS *******/

int touchAreaIndex = 0;
String touchStr;

struct touchAreaSt {
    int x1;
    int x2;
    int y1;
    int y2;
    String str;
};
 
void setup()
{
    Serial.begin(9600);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    tft.fillScreen(White);
    clearTouchButtons();
    createTouchButton("START",80,50,2,Black,Green,Red);
    createTouchButton("END",80,100,2,Black,Green,Red);
    createTouchButton("Cancel",80,150,2,Black,Green,Red);
    
} 
void loop()
{
   if(buttonPressed() == true) {
        Serial.println("Button Pressed.  ret String = " + touchStr);
        if( touchStr == "START" ){
            Serial.println(touchStr);
            productionOrderScreen2();
        } else if( touchStr == "END" ){
            Serial.println(touchStr);
        } else if (touchStr == "Cancel"){
             Serial.println(touchStr);
        }
   }
}



struct touchAreaSt touchArea[10];

void clearTouchButtons()
{
    for(int i=0; i <10; i++) {
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
    int len = msg.length();
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
    touchArea[touchAreaIndex].y1 = top;
    touchArea[touchAreaIndex].x2 = left + horiSize;
    touchArea[touchAreaIndex].y2 = top + vertSize;
    touchArea[touchAreaIndex].str = msg;
    touchAreaIndex++;

    tft.fillRoundRect(left, top, horiSize, vertSize, rectRadius, rectColor);
    tft.setCursor((cursorX ), (cursorY ));
    tft.setTextSize(textSize);
    tft.setTextColor( textColor );
    tft.print(msg);
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

        for(int i=0; i < touchAreaIndex; i++) {
             if (( p.x >= touchArea[i].x1 && p.x <= touchArea[i].x2 && p.y >= touchArea[i].y1 && p.y <= touchArea[i].y2 )) {
                touchStr = touchArea[i].str;
                //tft.fillRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 2, Red);
                 tftDisplay("SELECT Pressed ", 2, Yellow, 0, 0, false,Black);
                 touchAreaIndex = 0;
                //clearTouchButtons();
                return(true);
             }
        }
    }
    return false;
}
void center( String text, byte textSize )
{
    // if textSize(1)--> max char 53
    // if textSize(2)--> max char 26
    // if textSize(3)--> max char 17
    // if textSize(4)--> max char 13
    // if textSize(5)--> max char 10
    char space[6] = {0, 53, 26, 17, 13, 10};
    if(textSize == 0) {
        textSize = 1;
    }
    
    tft.setTextSize(textSize);
    int len = text.length();

    int x= ( space[textSize] / 2 ) - ( len / 2 );
    for(int i=0; i<=x ;i++){
        tft.print(" ");
    }
    tft.println(text);
}
void tftDisplay(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor)
{
    if( clearFlag==true) {
        tft.fillScreen(clearScreenColor);
    }
    tft.setCursor(x,y);
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    center(msg,textSize);
}
void tftDisplayNoCenter(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor)
{
    int msgLength = msg.length();
    tft.setCursor(x,y);
    tft.setTextSize(textSize);
    tft.setTextColor(color);
    if( clearFlag) {
        tft.fillScreen(clearScreenColor);
    }
    tft.print(msg);
}
void productionOrderScreen2() 
{
    Serial.println("before display");
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    Serial.println("After display");
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter("120" , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter("Tile",2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter("120",2,Yellow ,150,150, false , Black);

    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    createTouchButton("PREV",250,200,2,Black, Green , Red);
}
