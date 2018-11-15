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
/*
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
*/
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

String pONo1Str = "123456789012";
String materialForPONo1Str = "Tile";
String pONo2Str = "123456789012";
String materialForPONo2Str = "Mosaic";
unsigned int quantityForPONo1Count = 123;
unsigned int quantityForPONo2Count =456;

int touchAreaIndex = 0;
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
    tft.fillScreen(White);
    Serial.println(F("TFT Initialized"));
    clearTouchButtons();
//    productionOrderScreen1();
//    delay(3000);
//    productionOrderScreen2();
//    delay(3000);
//    confirmStartScreen();
//    delay(3000);
getProductionOrderDetailsScreen();
}
void loop() 
{
    /*
    if(buttonPressed() == true && touchStr == "SELECT") {
        Serial.println("Button Pressed.  ret String = " + touchStr);
        clearTouchButtons();
    }
    if(buttonPressed() == true && touchStr == "NEXT") {
        Serial.println("Button Pressed.  ret String = " + touchStr);
        productionOrderScreen2();
        clearTouchButtons();
    }
    if(buttonPressed() == true && touchStr == "PREV") {
        Serial.println("Button Pressed.  ret String = " + touchStr);
        productionOrderScreen1();
        clearTouchButtons();
    }  */   

   if(buttonPressed() == true ) {
         if( touchStr == "YES" ){
            Serial.println(touchStr);
            
        } else if( touchStr == "NO" ){
            Serial.println(touchStr);
            productionOrderScreen2();
        } 
    } 
 
}
void tftDisplay(String msg,int textSize,int color,int x, int y, boolean clearFlag,int clearScreenColor)
{
    int msgLength = msg.length();
    tft.setCursor(x,y);
    tft.setTextColor(color);
    if( clearFlag) {
        tft.fillScreen(clearScreenColor);
    }
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
    String str = "Button X1= " + String(left) + " Y1 = " +  String(top) + " x2 = " +  String(touchArea[touchAreaIndex].x2) + " Y2 = " + String(touchArea[touchAreaIndex].y2) + " Msg = " + msg ;
    Serial.println(str);
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

        for(int i=0; i < sizeof(touchArea)/sizeof(touchAreaSt); i++) {
             if (( p.x >= touchArea[i].x1 && p.x <= touchArea[i].x2 && p.y >= touchArea[i].y1 && p.y <= touchArea[i].y2 )) {
                touchStr = touchArea[i].str;
                tft.fillRoundRect(touchArea[i].x1, touchArea[i].y1, touchArea[i].x2-touchArea[i].x1, touchArea[i].y2 - touchArea[i].y1, 2, Red);
                clearTouchButtons();
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

void center( const char* text, int len, byte textSize )
{
    char space[6] = {0, 53, 26, 17, 13, 10};
    if(textSize == 0) {
        textSize = 1;
    }
    
    tft.setTextSize(textSize);
    int x= ( space[textSize] / 2 ) - ( len / 2 );
    for(int i=0; i<=x ;i++){
        tft.print(" ");
    }
    for(int i = 0; i < len; i++)
        tft.print(text[i]);
}

void productionOrderScreen1() 
{
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter(pONo1Str , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter(materialForPONo1Str,2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter("120",2,Yellow ,150,150, false , Black);

    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    createTouchButton("NEXT",250,200,2,Black, Green , Red);
    
}

void productionOrderScreen2() 
{
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true,Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false,Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplayNoCenter("PONo" , 2,Yellow ,10,90, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,85, false , Black);
    tftDisplayNoCenter(pONo2Str , 2,Yellow ,150,90, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,120, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,115, false , Black);
    tftDisplayNoCenter(materialForPONo2Str,2,Yellow ,150,120, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,150, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,145, false , Black);
    tftDisplayNoCenter("120",2,Yellow ,150,150, false , Black);

    createTouchButton("SELECT",150,200,2,Black, Green , Red);
    createTouchButton("PREV",250,200,2,Black, Green , Red);
    
}

void confirmStartScreen()
{
     tftDisplay("Check Raw Material" , 2,Yellow ,0,30, true , Black);
     tftDisplay("And" , 2,Yellow ,0,60, false , Black);
     tftDisplay("Readiness of the Machine" , 2,Yellow ,0,90, false , Black);
     tftDisplay("Press Start to Begin" , 2,Yellow ,0,120, false , Black);
     tftDisplay("Manufacturing" , 2,Yellow ,0,150, false , Black);
     createTouchButton("START",150,200,2,Black, Green , Red);
}


void getProductionOrderDetailsScreen()
{
    tftDisplay("You Have Selected" , 2,White ,10,10, true , Black);

    tftDisplayNoCenter("PONo" , 2,Yellow ,10,50, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,45, false , Black);
    tftDisplayNoCenter("12345" , 2,Yellow ,150,50, false , Black);

    tftDisplayNoCenter("Material" , 2,Yellow ,10,80, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,75, false , Black);
    tftDisplayNoCenter("Tile",2,Yellow ,150,80, false , Black);

    tftDisplayNoCenter("Quantity" , 2,Yellow ,10,110, false , Black);
    tftDisplayNoCenter(":" , 3 ,White ,120,105, false , Black);
    tftDisplayNoCenter("120",2,Yellow ,150,110, false , Black);

     tftDisplay("Press OK to Confirm",2,Yellow ,0,160, false , Black);
     createTouchButton("YES",120,200,2,Black, Green , Red);
     createTouchButton("NO",220,200,2,Black, Green , Red);
    
}

