void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    int val = analogRead(0);
    float raw = (1024.0/5.0);
    Serial.println(val);
    delay(1000);
}
