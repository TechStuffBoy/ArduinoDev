#include <inttypes.h>
#include <avr/io.h>

byte DO_00 = 37;
byte DO_01 = 36;
byte DO_02 = 35;
byte DO_03 = 34;

byte DO_04 = 33;
byte DO_05 = 32;
byte DO_06 = 31;
byte DO_07 = 30;

//PA0-->22,PA1-->23,PA2-->24,PA3-->25,PA4-->26,PA5-->27,PA6-->28,PA7-->29.
struct portStruct {
    int bitMask;
    byte pin;
    volatile unsigned char *hwPort;
    boolean curState;
    boolean prevState;
};

/*
portStruct diPorts[4] = {
                            {0b00000001, DI_00, PINC, false, false},
                            {0b00000010, DI_01, PINC, false, false},
                            {0b00000100, DI_02, PINC, false, false},
                            {0b00001000, DI_03, PINC, false, false}
                        };
*/
// DDRC = 0b11110000; //30,31,32,33, 34,35,36,37 -->for mega
//DDRA = 0b11111111 //29,28,27,26,25,24,23,22 --> for mega
portStruct doPorts[8] = {
                            {0b00000001, DO_00, &PORTC, false, false},
                            {0b00000010, DO_01, &PORTC, false, false},
                            {0b00000100, DO_02, &PORTC, false, false},
                            {0b00001000, DO_03, &PORTC, false, false},
                            {0b00010000, DO_04, &PORTC, false, false},
                            {0b00100000, DO_05, &PORTC, false, false},
                            {0b01000000, DO_06, &PORTC, false, false},
                            {0b10000000, DO_07, &PORTC, false, false}
                        };

void setup()
{
    Serial.begin(9600);
    DDRC = 0b11111111; //30,31,32,33, 34,35,36,37
    PORTC = 0x00;
    //DDRA = 0b11111111; //29,28,27,26,25,24,23,22
}

void loop()
{
    /*
    Serial.println("----------");
    Serial.println(PORTC |= (1<<0));
    Serial.println(PORTC |= (1<<1));
    Serial.println(PORTC |= (1<<2));
    Serial.println(PORTC |= (1<<3));
    Serial.println(PORTC |= (1<<4));
    Serial.println(PORTC |= (1<<5));
    Serial.println(PORTC |= (1<<6));
    Serial.println(PORTC |= (1<<7));
    Serial.println("----------");
    delay(3000);
    Serial.println(PORTC &= ~(1<<0));
    Serial.println(PORTC &= ~(1<<1));
    Serial.println(PORTC &= ~(1<<2));
    Serial.println(PORTC &= ~(1<<3));
    Serial.println(PORTC &= ~(1<<4));
    Serial.println(PORTC &= ~(1<<5));
    Serial.println(PORTC &= ~(1<<6));
    Serial.println(PORTC &= ~(1<<7));
    delay(3000);
    

    Serial.println(PORTC);
    Serial.println((1<<0));
    Serial.println((1<<1));
    Serial.println((1<<2));
    Serial.println((1<<3));
    Serial.println((1<<4));
    Serial.println((1<<5));
    Serial.println((1<<6));
    Serial.println((1<<7));
    Serial.println("<-------->");
//    Serial.println(PORTC |= (1<<0));
//    Serial.println(PORTC |= (1<<1));
//    Serial.println(PORTC |= (1<<2));
//    Serial.println(PORTC |= (1<<3));
//    Serial.println(PORTC |= (1<<4));
//    Serial.println(PORTC |= (1<<5));
//    Serial.println(PORTC |= (1<<6));
//    Serial.println(PORTC |= (1<<7));
    delay(2000);
    */
    //writeDOPort(DO_01,true);
//    writeDOPort(DO_01,false);
//    writeDOPort(DO_02,false);
//    writeDOPort(DO_03,false);
//    writeDOPort(DO_04,false);
//    writeDOPort(DO_05,false);
//    writeDOPort(DO_06,false);
//    writeDOPort(DO_07,false);
   
    /*
    writeDOPort(DO_00,true);
    writeDOPort(DO_01,true);
    writeDOPort(DO_02,true);
    writeDOPort(DO_03,true);
    writeDOPort(DO_04,true);
    writeDOPort(DO_05,true);
    writeDOPort(DO_06,true);
    writeDOPort(DO_07,true);
//    delay(1000);
//    writeDOPort(DO_00,false);
//    writeDOPort(DO_01,false);
//    delay(1000);
    */
//   doPorts[2].hwPort = doPorts[2].hwPort | 0b00000010 ;
//   PORTC = PORTC | 0b00000010;
     set_bit(&PORTC,0b11111111);
    writeDOPort(DO_00,true);
    writeDOPort(DO_01,true);
     
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

void set_bit(volatile unsigned char *port,int mask){
    //int mask = 0b11111111;
    *port = *port | mask;
}

