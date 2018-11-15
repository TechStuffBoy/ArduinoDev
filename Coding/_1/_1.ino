int sen1= 2;
int sen2=3;
int val1=0;
int val2=0;
int in=0;
int out=0;
int Actual=0;
unsigned long timer =0;
unsigned long intervel=1000;

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

  if(val1==HIGH && val2==LOW)
  {
    Serial.println("person is in");
    in++;
    delay(500);
    
  }
  if(val2==HIGH && val1==LOW)
  {  
    Serial.println("person is out");
    out++;
    delay(500);
    
  }
  if(val1==LOW && val2==LOW)
  {
    
  }
  if(val1==HIGH && val2==HIGH)
  {
    
  }
  Actual=in-out;
  Serial.print("Actual Persons inside :");
  Serial.println(Actual);
  delay(200);
  }


