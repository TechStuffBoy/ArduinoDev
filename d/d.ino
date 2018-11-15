boolean val1 = false;
boolean val2 = false;
void setup() {
    Serial.begin(9600);
  // put your setup code here, to run once:
pinMode(36,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 val1 = digitalRead(36);
 val2 = digitalRead(37);
 
   Serial.println("val1 =" + String(val1));
   Serial.println("val2 =" + String(val2));  
   delay(1000);
}
