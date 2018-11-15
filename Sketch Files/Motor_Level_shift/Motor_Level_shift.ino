int pin1=2;
int pin2=3;
void setup()
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2,OUTPUT);
}
void loop()
{
  digitalWrite( pin1, HIGH);
  digitalWrite( pin2, LOW);
  delay(3000);
  digitalWrite( pin1, LOW); 
  digitalWrite( pin2, HIGH);
  delay(3000);
}

