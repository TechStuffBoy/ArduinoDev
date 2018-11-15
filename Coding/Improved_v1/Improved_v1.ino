const int sen_1=2;
const int sen_2=3;
unsigned int val_1=0;
unsigned int val_2=0;
//unsigned int in=0;
//unsigned int out=0;
unsigned long previousMillis;
unsigned long currentMillis;
unsigned int count = 0;

const long interval=1000;

void setup()
{
  pinMode(sen_1,INPUT);
  pinMode(sen_2,INPUT);
  Serial.begin(9600);
}

void loop()
{
  val_1=digitalRead(sen_1);
  val_2=digitalRead(sen_2);
  in_condition();
  //out_condition();
}

void in_condition()
{
  previousMillis=0;
  count=0;
  while(1)
    {
      currentMillis = millis();
      if(currentMillis-previousMillis>= interval)
      {
        previousMillis=currentMillis;
        count++;
      }
      Serial.print("<");
      Serial.print(previousMillis);
      Serial.print(":");
      Serial.print(currentMillis);
      Serial.print(":");
      Serial.print(count);
      Serial.println(">");      
    }
  /*
  if(val_1)
  {
    while(!val_2)
    {
      currentMillis=millis();
      if(currentMillis-previousMillis>= interval)
      {
        previousMillis=currentMillis;
        count++;
      }
      if (count==5 )
        return 1;
      else 
        return 0; 
    }
  }
  if(val_2==HIGH)
  {
    Serial.println("person is in");
  }
  */
}

void out_condition()
{
  if(val_2)
  {
    while(!val_1)
    {
      unsigned long currentMillis=millis();
    if(currentMillis-previousMillis>= interval)
      {
        previousMillis=currentMillis;
      }
      break;
    }
  }
  if(val_1==HIGH)
  {
    Serial.println("Person is out");
  }
}

