#include <Adafruit_GFX.h>    // Core graphics library 
#include <SWTFT.h> // Hardware-specific library 
char dataRx;
// Assign readable names to some common 16-bit color values: 
#define    BLACK   0x0000 
#define    BLUE    0x001F 
#define    RED     0xF800 
#define    GREEN   0x07E0 
#define CYAN    0x07FF 
#define MAGENTA 0xF81F 
#define YELLOW  0xFFE0 
#define WHITE   0xFFFF
SWTFT tft;
void setup(void) { 
  Serial.begin(9600); 
  Serial.println(F("Type character in Arduino Serial port")); 
delay(1000);
  tft.reset();
  uint16_t identifier = tft.readID();
  
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  Serial.println(F("This is a serial data send Test!")); 
  Serial.println(F("Type character in Arduino serial port & press <SEND> ")); 
}
void loop(void) { 
  
    while (!Serial.available()); 
    dataRx=Serial.read(); 
    tft.setTextColor(YELLOW); 
    tft.setTextSize(2); 
tft.print(dataRx); 
    
    } 

