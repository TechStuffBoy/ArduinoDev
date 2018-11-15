#include <inttypes.h>
#include <avr/io.h>

byte DI_00 = 37; //Green
byte DI_01 = 36; //Red
byte DI_02 = 35;
byte DI_03 = 34;

byte DO_00 = 33;
byte DO_01 = 32;
byte DO_02 = 31;
byte DO_03 = 30;

//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
    boolean pullupFlag;
    volatile unsigned char *pullup;
};


portStruct diPorts[4] = {
                            {0b00000001, DI_00, &PINC, true,&PORTC},
                            {0b00000010, DI_01, &PINC, true, &PORTC },
                            {0b00000100, DI_02, &PINC, false, &PORTC},
                            {0b00001000, DI_03, &PINC, true, &PORTC}
                        };

// DDRC = 0b11110000; //30,31,32,33, 34,35,36,37 -->for mega
//DDRA = 0b11111111 //29,28,27,26,25,24,23,22 --> for mega
portStruct doPorts[4] = {
                            {0b00010000, DO_00, &PORTC },
                            {0b00100000, DO_01, &PORTC },
                            {0b01000000, DO_02, &PORTC },
                            {0b10000000, DO_03, &PORTC }
                        };

void setup()
{
    Serial.begin(9600);
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    //PORTC = 0b00001111;
    //DDRA = 0b11111111; //29,28,27,26,25,24,23,22
}

void loop()
{
    Serial.print("DI_00  DI_01 :");
    Serial.print(readDIPort(DI_00));
    Serial.print("  ");
    Serial.println(readDIPort(DI_01));

    /*
    if(readDIPort(DI_00))
        Serial.println("At DI_OO Flaot is at Top");
    else 
        Serial.println("At DI_OO Float is at Botton"); 

   if(!readDIPort(DI_01))
        Serial.println("At DI_01 Float is at Bottom");
    else
        Serial.println("At DI_01 Float is at Top"); 
     */ 
     writeDOPort(DO_00,true);  
    delay(2000);
    
}

boolean readDIPort(byte inputPin)
{
    boolean val;
    byte recd;
    //byte recd = PINC; //change where DI is connected
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        if(diPorts[i].pullupFlag){
            *diPorts[i].pullup |= diPorts[i].bitMask;
            recd = *diPorts[i].hwPort;
        } else
            recd = *diPorts[i].hwPort;
        if( diPorts[i].pin == inputPin ){
            val = (((recd & diPorts[i].bitMask) == 0)? false:true);
            return val;
        }
   }
}


void writeDOPort(byte port,boolean value) 
{
    for(unsigned int i=0; i< sizeof(doPorts)/sizeof(portStruct); i++) {
        if( doPorts[i].pin == port){
                if(value) {
                    *doPorts[i].hwPort = *doPorts[i].hwPort | doPorts[i].bitMask;
                }
                else{
                    *doPorts[i].hwPort &= ~(doPorts[i].bitMask);
                } 
            }
        }
}

