byte senPin = 3;
byte analog = 0 ;
void setup() {
    Serial.begin(9600);
    pinMode(senPin,INPUT);
 }

void loop() {
    Serial.print("Digital Val: ");
    Serial.print(digitalRead(senPin));
    Serial.print("  ANalog Val: ");
    Serial.println(analogRead(analog));
    delay(1000);
}
