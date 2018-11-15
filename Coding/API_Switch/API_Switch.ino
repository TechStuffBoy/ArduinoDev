byte switchState=1;
void setup()
{

  Serial.begin(9600);
  
}
void loop()
{
  if(Serial.available() > 21 )
  {
    
    if(Serial.read() == 0x7E)
    {
      for(int i=0;i<19;i++)
      {
      byte discardByte= Serial.read();
      }
      switchState= Serial.read();
       if( switchState == 0 )
       {
        Serial.println(" Switch open");
       }
       else if(switchState == 0x10 )
       {
        Serial.println(" Switch closed ");
       }
       else{}
      
  }
}
}

