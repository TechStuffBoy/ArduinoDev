long currentMillis1=0;
long previousMillis1=0;
long currentMillis2=0;
long previousMillis2=0;
int interval1=2000;
int interval2=2000;
void setup()
{
  Serial.begin(9600);
}
void loop()
{
    currentMillis1=millis();
   //previousMillis1=currentMillis;
   while(currentMillis1 - previousMillis1 > interval1){
    Serial.println("i am in  1");
    previousMillis1=currentMillis1;
    break;
   }
    currentMillis2=currentMillis2+currentMillis1;
    previousMillis2=previousMillis1;
    while(currentMillis2 - previousMillis2 > interval2){
    previousMillis2=currentMillis2;
    currentMillis1=previousMillis2;
    Serial.println("i am in  2");
    break;
   }
   
  
  }

