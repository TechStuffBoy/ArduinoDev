#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

// -- Setup

void setup(void) {
  
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);  // SDFP5408
  tft.setRotation(3); // Need for the Mega, please changed for your choice or rotation initial

}

void loop(void) {
  tft.fillScreen(BLACK);
  createCPSGatewayLogo(RED, 10, 10);
  while(1);
  delay(500);
//  tft.fillScreen(BLACK);
//  createCPSGatewayLogo(BLUE, 10, 200);
//  delay(500);
//  tft.fillScreen(BLACK);
//  createCPSGatewayLogo(MAGENTA, 150, 110);
//  delay(500);
//  tft.fillScreen(BLACK);
//  createCPSGatewayLogo(GREEN, 270, 10);
//  delay(500);
//  tft.fillScreen(BLACK);
//  createCPSGatewayLogo(YELLOW, 270, 200);
}

unsigned long createCPSGatewayLogo(uint16_t logoColor, int xLoc, int yLoc) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height();
  tft.fillScreen(BLACK);
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

//// Copy string from flash to serial port
//// Source string MUST be inside a PSTR() declaration!
//void progmemPrint(const char *str) {
//  char c;
//  while(c = pgm_read_byte(str++)) Serial.print(c);
//}
//
//// Same as above, with trailing newline
//void progmemPrintln(const char *str) {
//  progmemPrint(str);
//  Serial.println();
//}


