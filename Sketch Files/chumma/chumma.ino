int p2 = 3;
int Button = 4;
int p2Val = 0;
int p2Count = 0;
void setup()
{
  pinMode (Button, INPUT);
  pinMode (p2, INPUT);    
Serial.begin(9600);                                 
}

void loop() 
{
  //  Serial.println ("#############");
  
//  Serial.println (digitalRead(Button));
  
  if(digitalRead(Button) == HIGH)
  {
    p2Val = digitalRead(p2);
//    Serial.println (p2Val);
    if(p2Val == HIGH)
    {
      p2Count++;
      delay(1000);
      Serial.println(p2Count);
    }                               
   }
//   Serial.println(p2Count);                               
//   delay(1000);
  
}

