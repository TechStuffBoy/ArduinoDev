boolean curState1 = false;
boolean curState2 = false;
boolean curState3 = false;
boolean curState4 = false;
void setup() 
{
    DDRB = 0b00000011;
}



void loop()
{
    
if (PINC & (1 << PINC0)){
       curState1 = true;
 } else if ( PINC & (1 << PINC1)) {
    curState2 = true;
 } else if ( PINC & (1 << PINC2)) {
    curState3 = true;
 } else if (PINC & (1 << PINC3 )) {
    curState4 = true;
 }
 
}
