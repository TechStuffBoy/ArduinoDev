unsigned long currentMillis=0;
unsigned long previousMillis=0;
unsigned int interval=1000;

void setup(){
  Serial.begin(9600);
}
void loop(){
  currentMillis=millis();
  if(currentMillis-previousMillis>=interval)
  {
    Serial.println("I am in 1");
    previousMillis=currentMillis;
  }
  currentMillis=millis();
  if(currentMillis-previousMillis>=interval)
  {
    Serial.println("I am in 2");
    previousMillis=currentMillis;
  }
  
}

