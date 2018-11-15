unsigned int x = 0b1010001000110011;
uint8_t xlow = x & 0xff;
uint8_t xhigh = (x >> 8);
uint8_t xlow7bit = 0;
uint8_t xhigh7bit= 0;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() 
{
    xlow7bit = xlow & 0b01111111;
    xhigh7bit = xhigh & 0b01111111;
  // put your main code here, to run repeatedly:
    Serial.println(xlow);
    Serial.println(xhigh);

    Serial.println(xlow7bit);
    Serial.println(xhigh7bit);
    while(1)
    ;
}
