int sen1=0;
int val1=0;
long start,stopp,elapsed;
boolean condition=false;


void setup()
{
  Serial.begin(9600);
}
void loop()
{
  val1=analogRead(sen1);

  if(val1>0)
  {
    start= millis();
  if(val1<0)
    stopp=millis();
  elapsed=stopp-start;
  condition=true;
  }
if(condition==true)
{
   Serial.println(elapsed);
   delay(100);
   condition=false;
   }
}
