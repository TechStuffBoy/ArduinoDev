
int limit=0;
int analog[10]=0;
void setup() {
  Serial.begin(9600);
}

void loop() {
 if(Serial.available()>=21)
 {
  if(Serial.read()== 0x7E)
  {
    for(int i=1;i<19 ; i++)
    {
      byte Discard=Serial.read();
    }
    byte analogMSB=Serial.read();
    byte analogLSB=Serial.read();
    byte analogReading=(analogLSB + (analogMSB*256));
    smooth();
    limit=0;
    }

}
}
void smooth()
{
  for(limit=0;limit<10;limit++)
  {
    analog{limit]=analogReading;
    delay(10);
  }
   int averageV= analog[limit]/10;
   

