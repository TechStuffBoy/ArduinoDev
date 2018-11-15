#include <ArduinoJson.h>
#include <string.h>
#include <SPFD5408_Adafruit_GFX.h>    
#include <SPFD5408_Adafruit_TFTLCD.h> 
#include <SPFD5408_TouchScreen.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Calibrate values
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

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define MINPRESSURE 10
#define MAXPRESSURE 1000
const char* welcome ="Welcome to";
const char* swipe ="<Swipe Your Employee ID>";

void setup() {
    Serial.begin(9600);
    StaticJsonBuffer<350> jsonBuffer;
    String json= "{\n\"COMPANY\":\n{\n\"NAME\":\"Murudeswar Ceramics\",\n\"ID\":1111\n},\n\"PLANT\":\n{\n\"NAME\":\"Tumkur\",\n\"ID\":123\n},\n\"WORK_CENTER_1\":\n{\n\"NAME\":\"Kiln-1\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n},\n\"WORK_CENTER_2\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n}\n}";
    Serial.println(json);
    char json1[]= "{\n\"COMPANY\":\n{\n\"NAME\":\"Murudeswar Ceramics\",\n\"ID\":1111\n},\n\"PLANT\":\n{\n\"NAME\":\"Tumkur\",\n\"ID\":123\n},\n\"WORK_CENTER_1\":\n{\n\"NAME\":\"Kiln-1\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n},\n\"WORK_CENTER_2\":\n{\n\"NAME\":\"<WC_NAME>\",\n\"CODE\":1234,\n\"ID\":124,\n\"RDI_0\":2,\n\"RDO_0\":3,\n\"RAI_0\":1\n}\n}";
    JsonObject& root = jsonBuffer.parseObject(json1);
    if (!root.success()){
      Serial.println("parseObject() failed");
      return;
    }
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(3);
  //tft.setFont();
  tft.setFont(&FreeMonoBoldOblique12pt7b);
  
    
  const char* CompanyName = root["COMPANY"]["NAME"];
  int CompanyID = root["COMPANY"]["ID"];
  const char* PlantName = root["PLANT"]["NAME"];
  int PlantID = root["PLANT"]["ID"];
  const char* WorkCenterName_1 = root["WORK_CENTER_1"]["NAME"];
  int WorkCenterCode_1 = root["WORK_CENTER_1"]["CODE"];
  int WorkCenterID_1 = root["WORK_CENTER_1"]["ID"];
  int WC_1_RDI_0 = root["WORK_CENTER_1"]["RDI_0"];
  int WC_1_RDO_0 = root["WORK_CENTER_1"]["RDO_0"];
  int WC_1_RAI_0 = root["WORK_CENTER_1"]["RAI_0"];
  const char* WorkCenterName_2 = root["WORK_CENTER_2"]["NAME"];
  int WorkCenterCode_2 = root["WORK_CENTER_2"]["CODE"];
  int WorkCenterID_2 = root["WORK_CENTER_2"]["ID"];
  int WC_2_RDI_0 = root["WORK_CENTER_2"]["RDI_0"];
  int WC_2_RDO_0 = root["WORK_CENTER_2"]["RDO_0"];
  int WC_2_RAI_0 = root["WORK_CENTER_2"]["RAI_0"];
  tft.fillScreen(  0xFDAD);
  tft.setTextColor(BLACK);
  tft.setCursor(0,20);
  //tft.fillCircle(80,130,20,0xBB00);
  center(welcome,2);
  tft.setCursor(0,60);
  center(CompanyName,2);
  tft.setCursor(0,100);
  center(PlantName,2);
  tft.setCursor(0,140);
  center(WorkCenterName_1,2);
  tft.setCursor(0,180);
  center(swipe,2);
}

void loop(){
  
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

