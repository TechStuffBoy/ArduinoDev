
void setup()
{
  Serial.begin(9600);
  
}
void loop()
{
  if( Serial.available()>=21 )
{
     for(int i=0;i<22;i++)
    {
      Serial.print( Serial.read(), HEX);
      Serial.print( ",");
    }
     Serial.println();
}
}

