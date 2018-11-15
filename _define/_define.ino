#define WRITE  (PORTD = PORTD | 0x08;delay(2);DDRD = DDRD & 0x08;)
#define READ (DDRD = DDRD & 0x08;delay(2);PORTD = PORTD | 0x08);
void setup() {
  // put your setup code here, to run once:
    PORTD = PORTD | 0x08;
    delay(1000);
    DDRD = DDRD & 0x08;
}

void loop() {
  // put your main code here, to run repeatedly:

}
