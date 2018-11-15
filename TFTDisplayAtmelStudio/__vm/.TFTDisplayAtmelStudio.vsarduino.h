/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega w/ ATmega2560 (Mega 2560), Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 160
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 160
#define ARDUINO_AVR_MEGA2560
#define ARDUINO_ARCH_AVR
extern "C" void __cxa_pure_virtual() {;}

void setup(void);
void loop(void);
unsigned long testFillScreen();
unsigned long testText();
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();
void progmemPrint(const char *str);
void progmemPrintln(const char *str);

#include "D:\NANDHU\arduino-nightly-windows\arduino-nightly\hardware\arduino\avr\variants\mega\pins_arduino.h" 
#include "D:\NANDHU\arduino-nightly-windows\arduino-nightly\hardware\arduino\avr\cores\arduino\arduino.h"
#include <..\TFTDisplayAtmelStudio\TFTDisplayAtmelStudio.ino>
