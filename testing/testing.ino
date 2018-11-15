byte transmit = 13;
byte data = 170;
byte mask =1;
byte bitDelay = 1000;

void setup()
{
    Serial.begin(9600);
   pinMode(transmit,OUTPUT);
}
byte mask 
void loop()
{
  for (mask = 00000001; mask>0; mask <<= 1) { 
    Serial.println( mask, BIN);
    if (data & mask){ 
      Serial.println("Transmitting one");  
      digitalWrite(transmit,HIGH); // send 1
    }
    else{
        Serial.println("Transmitting zero");  
        digitalWrite(transmit,LOW); // send 0
     }
    delay(bitDelay); //delay
  }
}
