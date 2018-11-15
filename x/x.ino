
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
// Example 43.1.2
// tronixstuff.com/tutorials > chapter 43
// John Boxall - October 2011
 
void setup()
{
  DDRD = B11111111; // set PORTD (digital 7~0) to outputs
}
 
void loop()
{
  PORTD = B11111111;
  PORTD = B00000000;
  PORTD = B11111111;
  PORTD = B00000000;
  PORTD = B11111111;
  PORTD = B00000000;
  PORTD = B11111111;
  PORTD = B00000000;
  PORTD = B11111111;
  PORTD = B00000000;
}
