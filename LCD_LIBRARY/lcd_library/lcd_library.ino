#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define DECREMENT_CURSOR 0x04
#define INCREMENT_CURSOR 0x06
#define SHIFT_DISPLAY_RIGHT 0x05
#define SHIFT_DISPLAY_LEFT 0X07
#define DISPLAY_OFF_CURSOR_OFF 0X08
#define DISPLAY_OFF_CURSOR_ON 0X0A
#define DISPLAY_ON_CURSOR_OFF 0X0C
#define DISPLAY_ON_CURSOR_NO_BLINKING 0X0E
#define DISPLAY_ON_CURSOR_BLINKING 0X0F
#define SHIFT_CURSOR_TO_LEFT 0X10
#define SHIFT_CURSOR_TO_RIGHT 0X14
#define SHIFT_ENTIRE_DISPLAY_LEFT 0X18
#define SHIFT_ENTIRE_DISPLAY_RIGHT 0X1C
#define FORCE_CURSOR_TO_BEGINING_OF_FIRST_LINE 0X80
#define FORCE_CURSOR_TO_BEGINING_OF_SECOND_LINE 0XC0
#define EIGHT_BIT_MODE 0X38
#define FOUR_BIT_MODE 0X28

#define rs PB4 //10
#define rw PB5 //11
#define en PB6 //12
#define dataPort PORTB //(PB0->53)->D4,(PB1->52)->D5,(PB2->51)->D6,(PB3->50)->D7.

void initLcd4Bit();
//void initLcd8Bit();
void writeCommand4Bit(unsigned char command);
void writeData4Bit(unsigned char data);
void writeString4Bit(unsigned char *string);
void displayLeftShift();
void displayRightShift();
void clearDisplay();
void returnHome();
void setCursor(unsigned char pos1,unsigned char pos2);
void checkBusy();
void writePulse();
void readPulse(); 
void setup()
{
    Serial.begin(9600);
    initLcd4Bit(); 
    //DDRB = 0xFF; //PB0->53,PB1->52,PB2->51,PB3->50,PB4->10,PB5->11,PB6->12,PB7->13    
}

void loop()
{   
    setCursor(0,0);
    writeString4Bit("hello");
    setCursor(1,0);
    writeString4Bit("world");
    delay(1000);
    
}

void readPulse()
{
    // read is Positive edge Triggered so Low -> High
    PORTB = PORTB & (~(1<<en)); // clear or write zero to enable pin 
    delay(2);
    PORTB = PORTB | (1<<en); //set or write one to Enable pin
    Serial.println(" readPulse applied");
}

void writePulse()
{
    // Write is negative edge Triggered so High-> Low
    PORTB = PORTB | (1<<en); //set or write one to Enable pin
    delay(2);
    PORTB = PORTB & (~(1<<en)); // clear or write zero to enable pin
    Serial.println(" WritePulse applied"); 
}
void checkBusy()
{
    //busy flag is D7 which is connected to PB3
    //To check Busy Flag we should set rs=0,rw=1
    
    Serial.println("Checking For Busy Flag"); 
    DDRB = DDRB & (~(1<<3)); //write zero make as input
    PORTB = PORTB & ( ~(1<<rs) );// rs = 0
    PORTB = PORTB | (1<<rw); //rw=0
    while( ((PINB>>PB3)& 1) == 1 ) {
       readPulse();
    }
    DDRB = DDRB | (1<<3); //write one to make as output
    PORTB = PORTB & (~(1<<PB3));
    Serial.println("Busy Flag Cleared");
}
/*
void checkBusy()
{
    Serial.println("Checking For Busy Flag"); 
    //busy flag is D7 which is connected to PB3
    DDRB = DDRB & (~(1<<3)); //write zero make as input
    while( ((PORTB >> PB3)& 1) == 1 ) {
       readPulse(); 
    }
    DDRB = DDRB | (1<<3); //write one to make as output
    Serial.println("Busy Flag Cleared");  
}
*/
void writeCommand4Bit(unsigned char command)
{
    Serial.println("In writeCommand4Bit");
    // rs = 0, rw = 0
    checkBusy();
    PORTB = PORTB & ( ~( (1<<rs) | (1<<rw) ) );//rs = 0, rw = 0
    PORTB = ( PORTB & 0xF0 ) | ( (command>>4) & 0x0F); //send higher nibble
    Serial.println("Higher Nibble Send");
    writePulse();
    PORTB = ( PORTB & 0xF0 ) | ( command & 0x0F);//send lower nibble
    Serial.println("Lower Nibble Send");
    writePulse();
}

void writeData4Bit(unsigned char data)
{
    Serial.println("In writeData4Bit");
    //rs = 1, rw = 0;
    checkBusy();
    PORTB = PORTB | (1<<rs);
    PORTB = PORTB & (~(1<<rw));
    PORTB = ( PORTB & 0xF0 ) | ( (data>>4) & 0x0F); //send higher nibble
    Serial.println("Higher Nibble Send");
    writePulse();
    PORTB = ( PORTB & 0xF0 ) | ( data & 0x0F);//send lower nibble
    Serial.println("Lower Nibble Send");
    writePulse();
}

void writeString4Bit(const char *string)
{
    int i = 0;
    while( string[i] != '\0'){
        writeData4Bit(string[i]);
        i++;
    }
}

void setCursor(unsigned char pos1,unsigned char pos2)
{
    unsigned char address = 0;
    unsigned char LineAddrr = 0;
    if(pos1 == 0){
      LineAddrr = 0x80;  
    } else {
        LineAddrr = 0xC0;
    }
    address = LineAddrr + pos2;
    writeCommand4Bit(address);
    
}
void displayLeftShift()
{
    writeCommand4Bit(SHIFT_ENTIRE_DISPLAY_LEFT); 
}
void displayRightShift()
{
   writeCommand4Bit(SHIFT_ENTIRE_DISPLAY_RIGHT);  
}
void clearDisplay()
{
    writeCommand4Bit(CLEAR_DISPLAY);  
}
void returnHome()
{
   writeCommand4Bit(RETURN_HOME);  
}
void initLcd4Bit()
{
   Serial.println("LCD Initiated");
   DDRB = 0xFF; //PB0->53,PB1->52,PB2->51,PB3->50,PB4->10,PB5->11,PB6->12,PB7->13
   PORTB = 0x00;
   writeCommand4Bit(0x28);
   writeCommand4Bit(0x0E); // clear the screen
   writeCommand4Bit(0x01); // display on cursor on
   writeCommand4Bit(0x06); // increment cursor
   writeCommand4Bit(0x80); //row1,column1
}

