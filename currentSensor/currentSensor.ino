
unsigned int rawBits = 0;
double volts = 0.0;
void setup()
{
  Serial.begin(9600);
}


void loop()
{
  rawBits = analogRead(A0);
  volts = ((float)rawBits*5)/1023.0;
  float current = (2.5 - volts)/0.185;
  //float volt1 = volts/1023;
  Serial.print("Val is :");
  Serial.println(rawBits);
  Serial.print("Volts is:");
  Serial.println(volts);
  if(!(current < 0)){
    Serial.print("current is:");
    Serial.println(current);
  }
  delay(1000);
  
}

