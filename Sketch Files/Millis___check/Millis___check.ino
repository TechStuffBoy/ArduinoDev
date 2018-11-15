long lastReadTime;
long postingInterval;
long currentTime;
long check;
boolean con=true;
void setup()
{
  Serial.begin(9600);
  postingInterval=3000;
  currentTime = millis();
  Serial.print(" CurrentTime :");
  Serial.println(currentTime);
  lastReadTime=currentTime;
  Serial.print(" lastReadTime :");
  Serial.println(lastReadTime);
   Serial.println("End of Setup");
}

void loop() {
   
  check= currentTime-lastReadTime;
  Serial.print(" Check :");
  Serial.println(check); 
  currentTime = millis();
  Serial.print(" CurrentTime :");
  Serial.println(currentTime);
  
  while (currentTime-check>=postingInterval) {
     digitalWrite(13,HIGH);
     delay(200);
     digitalWrite(13,HIGH);
     delay(200);
     Serial.println("I am in Condition ");
     lastReadTime=currentTime-check;
     Serial.print(" lastReadTime :");
     Serial.println(lastReadTime);
    }
    
     
//    lastReadTime = millis();
//    Serial.print(" LastReadTime :");
//    Serial.println(lastReadTime); 
  }
