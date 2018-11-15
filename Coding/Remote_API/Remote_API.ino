
void setup() {
  Serial.begin(9600);
}

void loop() {
 if(Serial.available()>=21)
 {
  if(Serial.read()== 0x7E)
  {
    for(int i=0;i<19 ; i++)
    {
      byte Discard=Serial.read();
    }
    byte analogMSB=Serial.read();
    byte analogLSB=Serial.read();
    byte analogReading=(analogLSB + (analogMSB*256));
    Serial.print(analogReading);
    int voltage=((analogReading)*(3300/1024));
    int centi=(voltage-500)/10;
    int faren=((centi*1.8)+32);
    Serial.println("hello");
    Serial.println(centi);
    Serial.print(" C ");
    Serial.println(faren);
    Serial.print("F");
  
 }

}
}
