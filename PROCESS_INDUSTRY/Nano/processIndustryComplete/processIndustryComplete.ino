#include <inttypes.h>
#include <avr/io.h>
#include "HX711.h"

//DOUT is INPUT
//PD_SCK is INPUT

// Hx711.DOUT - pin #A1, INPUT
// Hx711.SCK - pin #A0, OUTPUT
//HX711 scale(DOUT, CLK);
 float value = 0;
 
byte DI_00 = 2; //PD2,ServoFwd
byte DI_01 = 3; //PD3,ServoRev
byte DI_02 = 4; //PD4,StepperFwd
byte DI_03 = 5; //PD5,StepperRev
byte DI_04 =  11; // loadCell DOUT

byte DO_00 = 7; //PD7
byte DO_01 = 8; //PB0
byte DO_02 = 9; //PB1
byte DO_03 = 10; //PB2
byte DO_04 = 12; //loadCell SCK

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
portStruct diPorts[5] = {
                            {0b00000100, DI_00, &PIND, false, &PORTD},
                            {0b00001000, DI_01, &PIND, false, &PORTD},
                            {0b00010000, DI_02, &PIND, false, &PORTD},
                            {0b00100000, DI_03, &PIND, false, &PORTD},
                            {0b00001000, DI_04, &PINB, false, &PORTB}
};



portStruct doPorts[5] = {
                            {0b10000000, DO_00, &PORTD},
                            {0b00000001, DO_01, &PORTB},
                            {0b00000010, DO_02, &PORTB},
                            {0b00000100, DO_03, &PORTB},
                            {0b00010000, DO_04, &PORTB}
                        };
// Hx711.DOUT - pin #A1, INPUT
// Hx711.SCK - pin #A0, OUTPUT
//HX711 scale(DOUT, CLK); //2,7
HX711 scale(DI_04, DO_04); //11,12
void setup()
{
    Serial.begin(9600);
	DDRD = 0b11000010; //7,6,5,4,3,2,1,0
    DDRB = DDRB | 0b00010111; //A7,A6,13,12,11,10,9,8
}

void loop()
{
   //Serial.print("read: \t\t");
   //Serial.println(scale.read());
   readCell_1();
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

void readCell_1()
{
  long preset = 915000;
  float grams = 100.0;
  int phoneWeight = 155;
  long analogPhoneWeight = 1145000;
  //float value = 0;
  float val = 0;
  val = (analogPhoneWeight - preset)/155;
  value = ((scale.read()-preset)/val);
  Serial.println(value);

}
