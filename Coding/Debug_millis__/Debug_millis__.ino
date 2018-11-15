unsigned long previousMillis=0;
unsigned long currentMillis;
unsigned int interval=1500;
unsigned int in=0;
unsigned int entering=0;
unsigned int leaving=0;
unsigned int total=0;
unsigned int out=0;
int sen1=4;
int sen2=5;
int val_1=0;
int val_2=0;
void setup()
{
 pinMode(sen1,INPUT);
 pinMode(sen2,INPUT);
 Serial.begin(9600);
}

void loop()
{
  val_1=digitalRead(sen1);
  val_2=digitalRead(sen2);
  if((val_1==HIGH && val_2==LOW))
  {
      currentMillis=millis();     
      previousMillis=currentMillis;
      in_condition();
  }

else if((val_2==HIGH && val_1==LOW))
{
  currentMillis=millis();
  previousMillis=currentMillis;
  out_condition();
}

else if((val_2 == HIGH && val_1 == HIGH))
{
  Serial.println("Both are High");
}

actual_strength();
}

void in_condition()
{
   Serial.println("In in_condition");
   while(val_2 != HIGH)
    { 
      
       currentMillis=millis();
        if( (currentMillis-previousMillis) >=interval)
        {
          Serial.println("Timeout..!!");
          break;
          }
       
         
     val_2=digitalRead(sen2);
    
      }
      if( val_2 == HIGH){
        Serial.println("Person In..!!");
        //in=1;
        total++;
      }
      
}

void out_condition()
{
  Serial.println("In out condition");
     val_1=0;
    while(val_1 !=HIGH)
    {
      currentMillis=millis();
      if((currentMillis-previousMillis)>=interval)
        {
          Serial.println("timeout...!");
          break;
        }
      val_1=digitalRead(sen1);
       }
     
      
      if(val_1==HIGH)
      {
        Serial.println("person out"); // if you put in if-else ststement then you can check only one sensors and if you put in only if condition then you shold check the status of all sensors connected.
       // out=1;
       total--;
      }
    
  }

  void actual_strength()
  {
//     if(in > out)
//        entering=1;
//     if(out>in)
//        leaving=1;   
//      
//        if((in+out)==2)
//          {
//            if(entering==1)
//             total=total+1;
//            else if(leaving==1)
//              total=total-1; 
//          }
//        Serial.println("Actual Strength in Room :");
//        Serial.println(total);
//        entering=0;
//        leaving=0;
//        in=0;
//        out=0;


Serial.print(" Total Strength :");
Serial.println(total);
    }

