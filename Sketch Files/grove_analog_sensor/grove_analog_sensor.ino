const int ledPin=13;
void setup() {
Serial.begin(9600);
pinMode(ledPin,OUTPUT);
} 
 
void loop() {
int sensorValue = analogRead(A0);
 Serial.println(sensorValue);
 delay(1000);
 if(sensorValue==1023)
  {
    digitalWrite(ledPin,HIGH);
  }
 else
 {
  digitalWrite(ledPin,LOW);
  }
}
