int sen1=2;
int sen2=3;
long start=0,stopp=0,elapsed1=0;
int sense1=0;
int sense2=0;
boolean enter= false;
void setup()
{
  Serial.begin(9600);
  pinMode(sen1,INPUT);
  pinMode(sen2,INPUT);
  
}
void loop()
{
 
  sense1=digitalRead(sen1);
  sense2=digitalRead(sen2);
 if(sense1>0)
 {
  start=millis();
 if(sense2>0)
   stopp=millis();
 elapsed1=stopp-start;
 }
//  if(sense2>0)
// {
//  start=millis();
//  if(sense2>0)
//   stopp=millis();
// elapsed1=stopp-start;
// }
// elapsed2=start-stopp;
 Serial.print(" Entering : ");
 Serial.print(elapsed1);
 Serial.println();
//Serial.print("out :");
// Serial.println("elapsed2);
 Serial.print("--------------------");
 Serial.println();
 delay(1000);

 
 
}


