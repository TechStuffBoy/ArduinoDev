#include <avr/interrupt.h>
volatile uint8_t count;

void setup()
{
	Serial.begin(9600);
	timer0Init();

}

void loop()
{

  Serial.print("Count Value :");
  Serial.println(count);

}

void timer0Init()
{
	//Prescalar = FCPU/1024
	TCCR0B |=((1<<CS02) | (1<<CS01));
	//Enable TimerOverFlow Flag
	TIMSK0 |= 1<<TOIE0;
	//Enable Global Interrupt also to enable peripheral Interrupt
	sei();
	//Initialize Timer Count to Zero
	TCNT0 = 0;
	count = 0;
	
}

ISR(TIMER0_OVF_vect)
{
	//This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
	//CPU automatically call this when TIMER0 overflows.

	//Increment our variable
	count++;
	if(count==61)
	{
		PORTC=~PORTC; //Invert the Value of PORTC
		count=0;
	}
}