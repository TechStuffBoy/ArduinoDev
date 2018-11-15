#include <inttypes.h>
#include <avr/io.h>

byte DI_00 = 37;
byte DI_01 = 36;
byte DI_02 = 35;
byte DI_03 = 34;

byte DO_00 = 33;
byte DO_01 = 32;
byte DO_02 = 31;
byte DO_03 = 30;

byte stepperFWD = 0;
byte stepperREV = 0;
int timeMilli = 2;

//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    byte bitMask;
    byte pin;
    volatile unsigned char *hwPort;
};


portStruct diPorts[4] = {
                            {0b00000001, DI_00, &PINC },
                            {0b00000010, DI_01, &PINC },
                            {0b00000100, DI_02, &PINC },
                            {0b00001000, DI_03, &PINC }
                        };

// DDRC = 0b11110000; //30,31,32,33, 34,35,36,37 -->for mega
//DDRA = 0b11111111 //29,28,27,26,25,24,23,22 --> for mega
portStruct doPorts[8] = {
                            {0b00010000, DO_00, &PORTC },
                            {0b00100000, DO_01, &PORTC },
                            {0b01000000, DO_02, &PORTC },
                            {0b10000000, DO_03, &PORTC }
                        };

void setup()
{
    Serial.begin(9600);
    DDRC = 0b11110000; //30,31,32,33, 34,35,36,37
    //DDRA = 0b11111111; //29,28,27,26,25,24,23,22
}

void loop()
{
  
  fwdStepper();
  
}

boolean readDIPort(byte inputPin)
{
    boolean val;
    //byte recd = PINC; //change where DI is connected
    for( unsigned int i=0; i< sizeof(diPorts)/sizeof(portStruct); i++) {
        byte recd = *diPorts[i].hwPort;
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

void stopStepper()
{
    writeDOPort(DO_00, false);
    writeDOPort(DO_01, false);
    writeDOPort(DO_02, false);
    writeDOPort(DO_03, false);
}

void fwdStepper()
{
        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);
}

void revStepper()
{
        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, false);
        writeDOPort(DO_03, true);
        delay(timeMilli);

        writeDOPort(DO_00, false);
        writeDOPort(DO_01, true);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);

        writeDOPort(DO_00, true);
        writeDOPort(DO_01, false);
        writeDOPort(DO_02, true);
        writeDOPort(DO_03, false);
        delay(timeMilli);
}
