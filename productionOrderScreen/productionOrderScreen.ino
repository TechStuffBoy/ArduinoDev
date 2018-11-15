#include <ArduinoJson.h>

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
// Instantiating TouchScreen and LCD
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define BOXSIZE 40
#define PENRADIUS 3
int i=0;
 
                   /******TFT LCD STUFF ENDS *******/
String poNoOneStr;
String materialDescOneStr;
unsigned quantityOneStr;

String poNoTwoStr;
String materialDescTwoStr;
unsigned int quantityTwoStr;

String poNoThreeStr;
String materialDescThreeStr;
unsigned int quantityThreeStr;
                   
//You can parse only the char array in arduinojson
char jsonStringBuff[] = "{\n\"ProductionOrderDetails\" : \n[\n{\n\"ProductionOrderNumber\" : \"1234\",\n\"MaterialDesc\" : \"Tile\",\n\"Quantity\" : 250\n},\n{\n\"ProductionOrderNumber\" : \"3453\",\n\"MaterialDesc\" : \"Mosaic\",\n\"Quantity\" : 170\n},\n{\n\"ProductionOrderNumber\" : \"4451\",\n\"MaterialDesc\" : \"Granite\",\n\"Quantity\" : 1250\n}\n]\n}";         
int currentPage = 0;
void setup()
{
    
    Serial.begin(9600);
    //Serial.println(jsonStringBuff);
    tft.reset();
    tft.begin(0x9341); // SDFP5408
    tft.setRotation(3);
    tft.fillScreen(White);
    Serial.println(F("TFT Initialized"));
    StaticJsonBuffer<400> jsonBuffer;
    
    JsonObject& root = jsonBuffer.parseObject(jsonStringBuff);
    const char* poNoOne = root["ProductionOrderDetails"][0]["ProductionOrderNumber"];
    const char* materialDescOne = root ["ProductionOrderDetails"][0]["MaterialDesc"];
    unsigned int quantityOne = root ["ProductionOrderDetails"][0]["Quantity"];
    
    const char* poNoTwo = root ["ProductionOrderDetails"][1]["ProductionOrderNumber"];
    const char* materialDescTwo = root ["ProductionOrderDetails"][1]["MaterialDesc"];
    unsigned int quantityTwo = root ["ProductionOrderDetails"][1]["Quantity"];
    
    const char* poNoThree = root ["ProductionOrderDetails"][2]["ProductionOrderNumber"];
    const char* materialDescThree = root ["ProductionOrderDetails"][2]["MaterialDesc"];
    unsigned int quantityThree = root ["ProductionOrderDetails"][2]["Quantity"];
    
    poNoOneStr = poNoOne;
    materialDescOneStr = materialDescOne;
    quantityOneStr = quantityOne;
    
    poNoTwoStr = poNoTwo;
    materialDescTwoStr = materialDescTwo;
    quantityTwoStr = quantityTwo;
    
    poNoThreeStr = poNoThree;
    materialDescThreeStr = materialDescThree;
    quantityThreeStr = quantityThree;

    Serial.print("poNoOneStr :");
    Serial.println(poNoOneStr);
    
    HomeScreen();
     
}

void loop()
{
    if (currentPage == 1) {
        TSPoint p = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        //if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
        {
            p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
            p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
            Serial.print("("); Serial.print(p.x);
            Serial.print(", "); Serial.print(p.y);
            Serial.println(")");
        //    if ( p.x >= 75 && p.x <= 200 && p.y >= 50 && p.y <= 80 ) {
        //        Serial.println("Touching Start Button");
        //        tft.drawRoundRect(100, 155, 135, 35, 4, Red);
        //    }
        //    if (!( p.x >= 75 && p.x <= 200 && p.y >= 50 && p.y <= 80 )) {
        //        tft.drawRoundRect(100, 155, 135, 35, 4, Green);
        //    }
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
//tftDisplay(workCenter_01_DescGlobal,2,Black,0,140, false);
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

void HomeScreen()
{
    currentPage = 1;
    tftDisplay("Murudeswar Ceramics ", 2, Yellow, 0, 0, true, Black);
    //tftDisplay("Pvt Ltd", 2, Yellow, 0, 30, false, Black);
    tftDisplay("ProductionOrder List", 2, White, 0, 30, false, Black);
    tft.drawLine(0,60,320,60, Red);
    tftDisplay("NUMBER MATERIAL QUANTITY ", 2, Yellow, 0, 90, false, Black);
    tft.setCursor (25,120);
    tft.setTextSize (2);
    tft.setTextColor( White);
    tft.print(poNoOneStr);
    tft.setCursor (125,120);
    tft.print(materialDescOneStr);
    tft.setCursor (225,120);
    tft.print(quantityOneStr);
    
    tft.setCursor (25,150);
    tft.print(poNoTwoStr);
    tft.setCursor (125,150);
    tft.print(materialDescTwoStr);
    tft.setCursor (225,150);
    tft.print(quantityTwoStr);
    
    tft.setCursor (25,180);
    tft.print(poNoThreeStr);
    tft.setCursor (125,180);
    tft.print(materialDescThreeStr);
    tft.setCursor (225,180);
    tft.print(quantityThreeStr);

   tft.drawRoundRect(0,116,310,20,2,Red);
   //tft.drawRoundRect(0,146,310,20,2,Red);
   //tft.drawRoundRect(0,176,310,20,2,Red);

   tft.fillRoundRect(20,210,115,20,2,Green);
   tft.setCursor (40,210);
   tft.setTextColor( Black);
   tft.print("Cancel");

   tft.fillRoundRect(165,210,115,20,2,Green);
   tft.setCursor (185,210);
   tft.setTextColor( Black);
   tft.print("Proceed");
}



