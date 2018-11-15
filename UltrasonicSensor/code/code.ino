/*
 * main.c
 *
 *  Created on: 11-Oct-2017
 *      Author: CPSG-WB-4
 */


#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define INDDR DDRB
#define OUTDDR DDRB
#define INPORT PORTB
#define OUTPORT PORTB
#define INPIN PINB
#define SWITCH PB2
#define LED PB5

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

void turnBit(volatile uint8_t *port,uint8_t pinNo,uint8_t value);
void setPinModee(volatile uint8_t *port,uint8_t pinNo, uint8_t pinMode);
void enablePullup(volatile uint8_t *port,uint8_t pinNo, uint8_t value);
uint8_t checkBitValue(volatile uint8_t *port,uint8_t pinNo);

int main()
{
//    setPinMode(&DDRB,SWITCH,INPUT);
    setPinModee(&DDRB,LED,OUTPUT);
//  turnBit(&PORTB,LED,LOW);
    while(1) {
        turnBit(&PORTB,LED,HIGH);
        _delay_ms(3000);
      turnBit(&PORTB,LED,LOW);
      _delay_ms(3000);

//      if( checkBitValue( &PINB, SWITCH) )
//          turnBit(&PORTB,LED,HIGH);
//      else
//          turnBit(&PORTB,LED,LOW);
    }
    return 0;
}


void turnBit(volatile uint8_t *port,uint8_t pinNo,uint8_t value)
{
    if(value)
        *port |= (1<<pinNo);//TurnOnBit
    else
        *port &= ~(1<<pinNo);//TurnOffBit
}

void setPinModee(volatile uint8_t *port,uint8_t pinNo, uint8_t pinMode)
{
    if(pinMode == OUTPUT)
        *port |= (1<<pinNo);//turnBitToOutput
    else if (pinMode == INPUT)
        *port &= ~(1<<pinNo); //turnBitToInput
}
void enablePullup(volatile uint8_t *port,uint8_t pinNo, uint8_t value)
{
    if(value)
        *port |= (1<<pinNo);//turnPullupresistor
}
uint8_t checkBitValue(volatile uint8_t *port,uint8_t pinNo)
{

    if((*port & (1<<pinNo)) == (1<<pinNo) ){
        return 1;
    } else
        return 0;
}

