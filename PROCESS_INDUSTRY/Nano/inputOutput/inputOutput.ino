#include <inttypes.h>
#include <avr/io.h>

byte DI_00 = 2; //PD2,ServoFwd
byte DI_01 = 3; //PD3,ServoRev
byte DI_02 = 4; //PD4,StepperFwd
byte DI_03 = 5; //PD5,StepperRev

byte DO_00 = 7; //PD7
byte DO_01 = 8; //PB0
byte DO_02 = 9; //PB1
byte DO_03 = 10; //PB2

//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
	boolean pullupFlag;
    volatile unsigned char *pullup;
};

//7,6,5,4,3,2,1,0
//A7,A6,13,12,11,10,9,8
portStruct diPorts[4] = {
                            {0b00000100, DI_00, &PIND, true, &PORTD},
                            {0b00001000, DI_01, &PIND, false, &PORTD},
                            {0b00010000, DI_02, &PIND, false, &PORTD},
                            {0b00100000, DI_03, &PIND, false, &PORTD}
};



portStruct doPorts[4] = {
                            {0b10000000, DO_00, &PORTD},
                            {0b00000001, DO_01, &PORTB},
                            {0b00000010, DO_02, &PORTB},
                            {0b00000100, DO_03, &PORTB}
                        };

void setup()
{
    Serial.begin(9600);
	DDRD = 0b11000010; //7,6,5,4,3,2,1,0
    DDRB = DDRB | 0b00000111; //A7,A6,13,12,11,10,9,8
}

void loop()
{
    if(!readDIPort(DI_00)){
        delay(25); //Debounce Delay /*MUST For BUTTONS*/
        if(!readDIPort(DI_00)){
            writeDOPort(DO_00,true);
            writeDOPort(DO_01,true);
        } 
    }else {
        writeDOPort(DO_00,false);
        writeDOPort(DO_01,false);
        }
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
