   
/****************************************************************************
    LCD_AVR_4f.ino  - Use an HD44780U based LCD with an Arduino
 
    Copyright (C) 2013 Donald Weiman    (weimandn@alfredstate.edu)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/****************************************************************************
         File:    LCD_AVR_4f.ino
         Date:    September 16, 2013
 
       Target:    Arduino (ATmega328)
     Compiler:    Arduino IDE (v1.0.5)
       Author:    Donald Weiman
       
      Summary:    4-bit data interface, busy flag not implemented.
                  Any LCD pin can be connected to any available I/O port.
                  Includes a simple write string routine.
*/
/******************************* Program Notes ******************************
 
            This program uses a 4-bit data interface and it uses the busy 
              flag to determine when the LCD controller is ready.  The LCD 
              RW line (pin 5) must therefore be connected to the uP.
 
            The use of the busy flag does not mean that all of the software 
              time delays have been eliminated.  There are still several 
              required in the LCD initialization routine where the busy flag  
              cannot yet be used.  These delays are have been implemented at 
              least twice as long as called for in the data sheet to 
              accommodate some of the out of spec displays that may show up.  
              There are also some other software time delays required to 
              implement timing requirements such as setup and hold times for 
              the various control signals.
 
  ***************************************************************************
 
            The four data lines as well as the three control lines may be
              implemented on any available I/O pin of any port.  These are
              the connections used for this program:
 
            The numbers shown next to the Arduino are the Arduino pin 
            numbers, not the IC pin numbers. 
 
                 -----------                   ----------
                |  Arduino  |                 |   LCD    |
                |           |                 |          |
                |        PD7|7  ------------->|D7        |
                |        PD6|6  ------------->|D6        |
                |        PD5|5  ------------->|D5        |
                |        PD4|4  ------------->|D4        |
                |           |                 |D3        |
                |           |                 |D2        |
                |           |                 |D1        |
                |           |                 |D0        |
                |           |                 |          |
                |        PB1|9  ------------->|E         |
                |        PB2|10 ------------->|RW        |
                |        PB0|8  ------------->|RS        |
                 -----------                   ----------
 
  **************************************************************************/

// LCD interface (should agree with the diagram above)
////   make sure that the LCD RW pin is connected to GND
//uint8_t  lcd_D7_ArdPin   =  7;                  // lcd D7 connection
//uint8_t  lcd_D6_ArdPin   =  6;
//uint8_t  lcd_D5_ArdPin   =  5;
//uint8_t  lcd_D4_ArdPin   =  4;
//
//uint8_t  lcd_E_ArdPin    =  9;                  // lcd Enable pin
//uint8_t  lcd_RS_ArdPin   =  8;                  // lcd Register Select pin
//uint8_t  lcd_RW_ArdPin   = 10;                  // lcd Read/Write pin

//   make sure that the LCD RW pin is connected to GND
uint8_t  lcd_D7_ArdPin   =  5;                  // lcd D7 connection
uint8_t  lcd_D6_ArdPin   =  4;
uint8_t  lcd_D5_ArdPin   =  3;
uint8_t  lcd_D4_ArdPin   =  2;

uint8_t  lcd_E_ArdPin    =  12;                  // lcd Enable pin
uint8_t  lcd_RS_ArdPin   =  10;                  // lcd Register Select pin
uint8_t  lcd_RW_ArdPin   = 8;                  // lcd Read/Write pin

// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2
//#define   lcd_LineThree   0x14                  // start of line 3 (20x4)
//#define   lcd_lineFour    0x54                  // start of line 4 (20x4)
//#define   lcd_LineThree   0x10                  // start of line 3 (16x4)
//#define   lcd_lineFour    0x50                  // start of line 4 (16x4)

// LCD instructions
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

// Program ID
uint8_t program_author[]   = "Donald Weiman";
uint8_t program_version[]  = "LCD_AVR_4f (ino)";
uint8_t program_date[]     = "Sep 16, 2013";


/******************************* Main Program Code *************************/

void setup (void)
{
// configure the microprocessor pins for the data lines
    pinMode(lcd_D7_ArdPin, OUTPUT);                 // 8 data lines - output
    pinMode(lcd_D6_ArdPin, OUTPUT);
    pinMode(lcd_D5_ArdPin, OUTPUT);
    pinMode(lcd_D4_ArdPin, OUTPUT);

// configure the microprocessor pins for the control lines
    pinMode(lcd_E_ArdPin, OUTPUT);                  // E line - output
    pinMode(lcd_RS_ArdPin, OUTPUT);                 // RS line - output
    pinMode(lcd_RW_ArdPin, OUTPUT);                 // RW line - output    

// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4f();                                  // initialize the LCD display for an 8-bit interface
      
// display the first line of information
    lcd_write_string_4f(program_author);

// set cursor to start of second line
        lcd_check_BF_4();                           // make sure LCD controller is ready
    lcd_write_instruction_4f(lcd_SetCursor | lcd_LineTwo);
 
// display the second line of information
    lcd_write_string_4f(program_version); 
}

// endless loop
void loop()
  {  
  }
/******************************* End of Main Program Code ******************/

/*============================== 8-bit LCD Functions ======================*/
/*
  Name:     lcd_init_4f
  Purpose:  initialize the LCD module for a 8-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses the busy flag instead of time delays when possible
*/
void lcd_init_4f(void)
{
// Power-up delay
    delay(100);                                     // initial 40 mSec delay

 // IMPORTANT - At this point the LCD module is in the 8-bit mode and it is expecting to receive  
//   8 bits of data, one bit on each of its 8 data lines, each time the 'E' line is pulsed.
//
// Since the LCD module is wired for the 4-bit mode, only the upper four data lines are connected to 
//   the microprocessor and the lower four data lines are typically left open.  Therefore, when 
//   the 'E' line is pulsed, the LCD controller will read whatever data has been set up on the upper 
//   four data lines and the lower four data lines will be high (due to internal pull-up circuitry).
//
// Fortunately the 'FunctionReset' instruction does not care about what is on the lower four bits so  
//   this instruction can be sent on just the four available data lines and it will be interpreted 
//   properly by the LCD controller.  The 'lcd_write_4' subroutine will accomplish this if the 
//   control lines have previously been configured properly.

// Set up the RS and E lines for the 'lcd_write_4' subroutine.
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low    
    
// Reset the LCD controller
    lcd_write_4(lcd_FunctionReset);                 // first part of reset sequence
    delay(10);                                      // 4.1 mS delay (min)

    lcd_write_4(lcd_FunctionReset);                 // second part of reset sequence
    delayMicroseconds(200);                         // 100uS delay (min)

    lcd_write_4(lcd_FunctionReset);                 // third part of reset sequence
    delayMicroseconds(200);                         // this delay is omitted in the data sheet

// Preliminary Function Set instruction - used only to set the 4-bit mode.
// The number of lines or the font cannot be set at this time since the controller is still in the
//  8-bit mode, but the data transfer mode can be changed since this parameter is determined by one 
//  of the upper four bits of the instruction.  

    lcd_write_4(lcd_FunctionSet4bit);               // set mode, lines, and font
    delayMicroseconds(80);                          // 40 uS delay (min) 

// Function Set instruction
    lcd_write_instruction_4f(lcd_FunctionSet4bit);  // set mode, lines, and font
// --> from this point on the busy flag is available <--  
    
// The next three instructions are specified in the data sheet as part of the initialization routine, 
//  so it is a good idea (but probably not necessary) to do them just as specified and then redo them 
//  later if the application requires a different configuration.

// Display On/Off Control instruction
    lcd_check_BF_4();                               // make sure LCD controller is ready
    lcd_write_instruction_4f(lcd_DisplayOff);       // turn display OFF

// Clear Display instruction
    lcd_check_BF_4();                               // make sure LCD controller is ready
    lcd_write_instruction_4f(lcd_Clear);            // clear display RAM

// ; Entry Mode Set instruction
    lcd_check_BF_4();                               // make sure LCD controller is ready
    lcd_write_instruction_4f(lcd_EntryMode);        // set desired shift characteristics

// This is the end of the LCD controller initialization as specified in the data sheet, but the display
//  has been left in the OFF condition.  This is a good time to turn the display back ON.

// Display On/Off Control instruction
    lcd_check_BF_4();                               // make sure LCD controller is ready
    lcd_write_instruction_4f(lcd_DisplayOn);        // turn the display ON   
}

/*...........................................................................
  Name:     lcd_write_string_4f
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses the busy flag instead of time delays 
*/
void lcd_write_string_4f(uint8_t theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_check_BF_4();                           // make sure LCD controller is ready    
        lcd_write_character_4f(theString[i]);
        i++;
    }
}

/*...........................................................................
  Name:     lcd_write_character_4f
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    configures RW (busy flag is implemented)
*/
void lcd_write_character_4f(uint8_t  theData)
{
    digitalWrite(lcd_RW_ArdPin, LOW);               // write to LCD module (RW low)
    digitalWrite(lcd_RS_ArdPin, HIGH);              // select the Data Register (RS high)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theData);                           // write the upper 4-bits of the data
    lcd_write_4(theData << 4);                      // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_instruction_4f
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    configures RW (busy flag is implemented)
*/
void lcd_write_instruction_4f(uint8_t  theInstruction)
{
    digitalWrite(lcd_RW_ArdPin, LOW);               // write to LCD module (RW low)
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_E_ArdPin, LOW);                // make sure E is initially low
    lcd_write_4(theInstruction);                    // write the upper 4-bits of the data
    lcd_write_4(theInstruction << 4);               // write the lower 4-bits of the data
}

/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a nibble of information to the LCD module
  Entry:    (theByte) is the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    the desired information (4-bits) must be in the upper half of (theByte)
            use either time delays or the busy flag
*/
void lcd_write_4(uint8_t  theByte)
{
    digitalWrite(lcd_D7_ArdPin,LOW);                       // assume that data is '0'
    if (theByte & 1<<7) digitalWrite(lcd_D7_ArdPin,HIGH);  // make data = '1' if necessary
          
    digitalWrite(lcd_D6_ArdPin,LOW);                       // repeat for each data bit
    if (theByte & 1<<6) digitalWrite(lcd_D6_ArdPin,HIGH);    

    digitalWrite(lcd_D5_ArdPin,LOW);
    if (theByte & 1<<5) digitalWrite(lcd_D5_ArdPin,HIGH);

    digitalWrite(lcd_D4_ArdPin,LOW);
    if (theByte & 1<<4) digitalWrite(lcd_D4_ArdPin,HIGH);
    
// write the data
                                                    // 'Address set-up time' (40 nS)
    digitalWrite(lcd_E_ArdPin,HIGH);                // Enable pin high
    delayMicroseconds(1);                           // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    digitalWrite(lcd_E_ArdPin,LOW);                 // Enable pin low
    delayMicroseconds(1);                           // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}

/*...........................................................................
  Name:     lcd_check_BF_4
  Purpose:  check busy flag, wait until LCD is ready
  Entry:    no parameters
  Exit:     no parameters
  Notes:    main program will hang if LCD module is defective or missing
            data is read while 'E' is high
*/
void lcd_check_BF_4(void)
{
    uint8_t busy_flag_copy;                         // busy flag 'mirror'

    pinMode(lcd_D7_ArdPin, INPUT);                  // set D7 data direction to input
    digitalWrite(lcd_RS_ArdPin, LOW);               // select the Instruction Register (RS low)
    digitalWrite(lcd_RW_ArdPin, HIGH);              // read from the LCD module (RW high)

    do
    {
        busy_flag_copy = 0;                         // initialize busy flag 'mirror'      
        digitalWrite(lcd_E_ArdPin,HIGH);            // Enable pin high
        delayMicroseconds(1);                       // implement 'Delay data time' (160 nS) and 'Enable pulse width' (230 nS)
 
        busy_flag_copy = digitalRead(lcd_D7_ArdPin);   // get actual busy flag status 

        digitalWrite(lcd_E_ArdPin,LOW);             // Enable pin low
        delayMicroseconds(1);                       // implement 'Address hold time' (10 nS), 'Data hold time' (10 nS), and 'Enable cycle time' (500 nS )

// read and discard alternate nibbles (D3 information)
        digitalWrite(lcd_E_ArdPin,HIGH);            // Enable pin high
        delayMicroseconds(1);                       // implement 'Delay data time' (160 nS) and 'Enable pulse width' (230 nS)
        digitalWrite(lcd_E_ArdPin,LOW);             // Enable pin low
        delayMicroseconds(1);                       // implement 'Address hold time' (10 nS), 'Data hold time' (10 nS), and 'Enable cycle time' (500 nS )

    }   while (busy_flag_copy);                     // check again if busy flag is high    

// arrive here if busy flag is clear -  clean up and return
    digitalWrite(lcd_RW_ArdPin, LOW);               // write to the LCD module (RW low)
    pinMode(lcd_D7_ArdPin, OUTPUT);                 // set D7 data direction to output
}

