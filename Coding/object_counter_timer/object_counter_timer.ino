#include"Wire.h"
#define DS1307_I2C_ADDRESS 0x68
unsigned long previousMillis=0;
unsigned long currentMillis;
unsigned int interval=1500;
unsigned int total=0;
int sen1=4;
int sen2=5;
int val_1=0;
int val_2=0;
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setup()
{
 pinMode(sen1,INPUT);
 pinMode(sen2,INPUT);
 Serial.begin(9600);
 Wire.begin();
}

void loop()
{
  object_counter();
  
}


void object_counter()
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
  unsigned int prev=0;
  prev=total;
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
      if(prev!=total){
        Serial.println(" change occured ");
        Serial.print("Someone Entered In:");
        displayTime();
      }
}

void out_condition()
{
  unsigned int prev=0;
  prev=total;
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
      if(prev!=total){
        Serial.println(" change occured ");
        Serial.print("Someone Leaved out:");
        displayTime();
      }
    
  }

  void actual_strength()
  {
    Serial.print(" Total Strength :");
    Serial.println(total);
    }
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS1307time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}

 void readDS1307time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
} 

