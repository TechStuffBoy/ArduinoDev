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

int TotalTiles=5;
int FinishedTiles=0;
int RemainingTiles=5;
int RejectedTiles=0;

#define BOXSIZE 40
#define PENRADIUS 3
long WorkOrder1=123456;
long WorkOrder2=654321;
long WorkOrder3=123321;
int CurrentPage=0;
int w1=0;



void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  tft.reset();
  tft.begin(0x9341); // SDFP5408
  tft.setRotation(1); // Need for the Mega, please changed for your choice or rotation initial
  HomeScreen();
  //WorkOrderSuceed(WorkOrder1);
 
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop(){
  if(CurrentPage == 0){

  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  if( p.x > 150 && p.x< 200 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder1 ");
    w1=0;
    WorkOrder(WorkOrder1);
  }

   if( p.x > 85 && p.x< 145 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder2 ");
    w1=1;
    WorkOrder(WorkOrder2);
   // WorkOrder2();
  }
  if( p.x > 20 && p.x< 70 && p.y > 135 && p.y <235 ){
    Serial.println("In Workorder3 ");
    w1=2;
    WorkOrder(WorkOrder3);
   // WorkOrder3();
   }
}
}

if(CurrentPage == 1 ){

  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  if( p.x > 40 && p.x< 100 && p.y > 20 && p.y <120 ){
    HomeScreen();
  }
  if( p.x > 20 && p.x< 70 && p.y > 135 && p.y <235 ){
    Serial.println("WorkOrder Proceed Page");
    if(w1 == 0){
     WorkOrderSuceed(WorkOrder1);
       }

       if(w1 == 1){
     WorkOrderSuceed(WorkOrder2);
       }

       if(w1 == 2){
     WorkOrderSuceed(WorkOrder3);
       }
}
}
}
if(CurrentPage == 2 ){

  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
}
}
if(CurrentPage == 3 ){

  TSPoint p=ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if(p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");

  if( p.x > 20 && p.x< 70 && p.y > 135 && p.y <235 ){
    Serial.println("WorkOrder Proceed Page");
   HomeScreen();
   }
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

void HomeScreen(){
  CurrentPage=0; 
  tft.fillScreen(Black);
  //TFT has 320x240 pixel 
  tft.setCursor (65,10);
  tft.setTextSize (3);
  tft.setTextColor(Yellow);
  tft.println("CPS GATEWAY");
  
  tft.setCursor (40,35);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.println("Technologies Pvt Ltd");
  // tft.drawLine(60, 60, 80,80, Red);
  tft.drawLine(0,60,320,60, Red);
  
  // WorkOrder1_GUI_Starts
  tft.drawRoundRect(15,80,135,35,4,White);
  tft.fillRoundRect(14,78,133,33,3,Green);
  
  tft.setCursor (25,87);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("WorkOrder1");
  
  tft.setCursor (150,72);
  tft.setTextSize (7);
  tft.setTextColor(White);
  tft.println(":");
  
  tft.drawRoundRect(180,85,125,30,4,White);
  tft.fillRoundRect(177,83,124,30,3,Green);
  
  tft.setCursor (210,89);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print(WorkOrder1);
  
  // WorkOrder1_GUI_Stops
  
  // WorkOrder2_GUI_Startss
  tft.drawRoundRect(15,130,135,35,4,White);
  tft.fillRoundRect(14,128,134,34,3,Green);
  
  tft.setCursor (25,140);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("WorkOrder2");
  
  tft.setCursor (150,124);
  tft.setTextSize (7);
  tft.setTextColor(White);
  tft.println(":");
  
  tft.drawRoundRect(180,130,125,30,4,White);
  tft.fillRoundRect(177,128,124,30,3,Green);
  
  tft.setCursor (200,133);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print(WorkOrder2);
  // WorkOrder2_GUI_Stops
  
  // WorkOrder3_GUI_Startss
  tft.drawRoundRect(15,185,135,35,4,White);
  tft.fillRoundRect(14,183,134,34,3,Green);
  
  tft.setCursor (25,190);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("WorkOrder3");
  
  tft.setCursor (150,180);
  tft.setTextSize (7);
  tft.setTextColor(White);
  tft.println(":");
  
  tft.drawRoundRect(180,186,125,30,4,White);
  tft.fillRoundRect(177,184,124,30,3,Green);
  
  tft.setCursor (200,190);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print(WorkOrder3);
 // WorkOrder3_GUI_Stops
 }

void WorkOrder(long order){
  CurrentPage=1;
 
  tft.fillScreen(Black);
  //TFT has 320x240 pixel 
  tft.setCursor (65,10);
  tft.setTextSize (3);
  tft.setTextColor(Yellow);
  tft.println("CPS GATEWAY");
  tft.setCursor (40,35);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.println("Technologies Pvt Ltd");
  // tft.drawLine(60, 60, 80,80, Red);
  tft.drawLine(0,60,320,60, Red);
  tft.setCursor (25,80);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.print("YOU ARE ABOUT TO CHOOSE\n\n     WORKORDER:");
  tft.println(order);

  tft.drawRoundRect(15,165,135,35,4,White);
  tft.fillRoundRect(14,163,134,34,3,Green);
  
  tft.setCursor (25,170);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("  CANCEL");
  
//  tft.setCursor (150,180);
//  tft.setTextSize (7);
//  tft.setTextColor(White);
//  tft.println(":");
  
  tft.drawRoundRect(180,166,125,30,4,White);
  tft.fillRoundRect(177,164,124,30,3,Green);
  
  tft.setCursor (200,170);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("PROCEED");
}

void WorkOrderSuceed(long order){
  CurrentPage=2;
  tft.fillScreen(Black);
  //TFT has 320x240 pixel 
  tft.setCursor (65,10);
  tft.setTextSize (3);
  tft.setTextColor(Yellow);
  tft.println("CPS GATEWAY");
  tft.setCursor (40,35);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.println("Technologies Pvt Ltd");
  // tft.drawLine(60, 60, 80,80, Red);
  tft.drawLine(0,60,320,60, Red);

  tft.setCursor (5,80);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.print("     WORKORDER:");
  tft.print(order);
  

  tft.setCursor (20,100);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print("   TOTAL TILES :");
  tft.setCursor (205,115);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(TotalTiles);
  

  tft.setCursor (20,130);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print(" FINISHED TILES:");
  tft.setCursor (205,145);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(FinishedTiles);
  
  tft.setCursor (20,160);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print(" REJECTED TILES:");
  tft.setCursor (205,175);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(RejectedTiles);
  
  tft.setCursor (20,190);
  tft.setTextSize (2);
  tft.setTextColor(Red);
  tft.println();
  tft.print(" REMAINING TILES:");
  tft.setCursor (205,205);
  tft.setTextSize (2);
  tft.setTextColor(Red);
  tft.print(RemainingTiles);
  int i=0;
  while(i<TotalTiles){
  i++;
  FinishedTiles++;
  RemainingTiles=TotalTiles-FinishedTiles;

  tft.setCursor (205,145);
  tft.setTextSize (2);
  tft.setTextColor(Green,Black);
  tft.print(FinishedTiles);

  tft.setCursor (205,145);
  tft.setTextColor(Black);
  tft.write('\n');

  tft.setCursor (205,205);
  tft.setTextSize (2);
  tft.setTextColor(Red,Black);
  tft.print(RemainingTiles);

  delay(1000);
}
 if((TotalTiles - FinishedTiles) == 0 ){
  Success(order);
 }
 RemainingTiles=5;
 FinishedTiles=0;
 }
void Success(long order){
  CurrentPage=3;
  tft.fillScreen(Black);
  //TFT has 320x240 pixel 
  tft.setCursor (65,10);
  tft.setTextSize (3);
  tft.setTextColor(Yellow);
  tft.println("CPS GATEWAY");
  tft.setCursor (40,35);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.println("Technologies Pvt Ltd");
  // tft.drawLine(60, 60, 80,80, Red);
  tft.drawLine(0,60,320,60, Red);

  tft.setCursor (40,65);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.print("   WORKORDER:");
  tft.println(order);
  tft.setCursor (40,75);
  tft.setTextSize (2);
  tft.setTextColor(White);
  tft.print("\n          COMPLETED");
 
  tft.setCursor (20,100);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print("  TOTAL TILES   :");
  tft.setCursor (205,115);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(TotalTiles);
  

  tft.setCursor (20,130);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print("  FINISHED TILES:");
  tft.setCursor (205,145);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(FinishedTiles);
  
  tft.setCursor (20,160);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.println();
  tft.print("  REJECTED TILES:");
  tft.setCursor (205,175);
  tft.setTextSize (2);
  tft.setTextColor(Green);
  tft.print(RejectedTiles);


  tft.drawRoundRect(180,196,125,30,4,White);
  tft.fillRoundRect(177,194,124,30,3,Green);
  
  tft.setCursor (220,200);
  tft.setTextSize (2);
  tft.setTextColor(Black);
  tft.print("HOME");
 
}

