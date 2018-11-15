void setup()
{
    Serial.begin(9600);
}

void loop()
{
    while(Serial.available()== 0);
    if(Serial.available()){
        int c = Serial.parseInt();
        if( c == 22 ){
            Serial.println("22");
            digitalWrite(13,HIGH);
        }
  }
}

