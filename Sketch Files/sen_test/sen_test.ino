int sen=3;
int val=0;
int count=0;
long currentMillis=0;
long previousMillis=0;
long interval=1000;
void setup()
{
  pinMode(sen,INPUT);
  Serial.begin(9600);
  }
void loop()
{
  val=digitalRead(sen);
  if(val){
    currentMillis=millis();
    if(currentMillis-previousMillis>=interval)
  {
       count++;
       previousMillis=currentMillis;
  }
    Serial.print(currentMillis-previousMillis);
    Serial.print("Found Object");
    Serial.println(count);
    // delay(100);
  }
   
}

