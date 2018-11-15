int sen1= 2;
int sen2=3;
int val1=0;
int val2=0;
int in=0;
int previous_in=0;
int previous_out=0;
int inside_room=0;
int out=0;
unsigned long currentMillis=0;
unsigned long previousMillis=0;
int previous_inside_room=0;
unsigned long intervel=2000;
boolean casee1=true;
boolean casee2=false;

void setup()
{
  Serial.begin(9600);
  pinMode(sen1, INPUT);
  pinMode(sen2, INPUT);
  
}

void loop() 
{
  val1=digitalRead(sen1);
  val2=digitalRead(sen2);
  
if(casee1)
{ 
   if(val1== HIGH)
  {
   currentMillis=millis();
   
   if(((currentMillis - previousMillis)>=intervel) && val2 == HIGH )
   {
      Serial.println("Person is in ");
      in++;
   }
   else
      Serial.println("Person didn't Enter");
   
   previousMillis=millis();
   }
 //  person_count();
   casee1=false;
   casee2=true;
} 

if(casee2==true)
{

   if(val2== HIGH)
  {
   currentMillis=millis();
  
   if(((currentMillis - previousMillis)>=intervel) && val1== HIGH )
   {
    
      Serial.println("Person is out ");
      out++;
   }
   
    else
      Serial.println("Person didn't go out");
   
   previousMillis=millis();
   }
  // person_count();
   casee2=false;
   casee1=true;
}


if(val1==HIGH && val2==HIGH )
{
  Serial.println(" Collision Happening");
  Serial.println("Reverting to previous State");
  previous_in=in;
  previous_out=out;
  previous_inside_room=previous_in-previous_out;
  Serial.println(" Current persons in room :");
  Serial.print(previous_inside_room);
  if(val1== HIGH)
  {
    in++;
    }
   else
    out++; 
}
if(val1==LOW && val2== LOW )
{
  //Do nothing.
}
}


void person_count()
{
  inside_room=in-out;
  Serial.println(" No of persons inside room :");
  Serial.print(inside_room);
  Serial.println();
}

