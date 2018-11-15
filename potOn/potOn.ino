void setup() {
  // put your setup code here, to run once:
    DDRC = 11111111;
    Serial.begin(9600);
}

void loop() {
    Serial.println(PORTC |= (1<<6));
    delay(1000);
    Serial.println(PORTC &= ~(1<<6));
    delay(1000);
  // put your main code here, to run repeatedly:

}
