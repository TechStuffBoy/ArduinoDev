/*
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
*/

#include<SPI.h>
#include<SD.h>
int senPin=0;
int potValue=0;
int Value=0;


File File1;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing Sd Card...");
 if(!SD.begin())
 {
   Serial.println("Initialization Failed!!");
   return;
  }
  Serial.println("Initialization Successfull :) ");

File1=SD.open( "sensor.txt",FILE_WRITE);

if(File1)
{
  Serial.println(" I am in File1_sensor.txt "); 
  File1.println(" I am in File1_sensor.txt ");
  File1.close();
}
else
  Serial.println(" Error opening 'sensor.txt' " );
}
void loop()
{
  Value=read_pot();
  print_SD();
  read_From_SD();
 
  
}
void read_pot()
{
 potValue=analogRead(senPin);
 potValue=map(potValue,0,1023,0,255);
 return potValue;
}
void print_SD()
{
  File1=SD.open( "sensor.txt",FILE_WRITE);
  if(File1)
{
  Serial.println("PotValue :" ); 
  File1.print("PotValue :");
  File1.println(Value);
  File1.close();
}
else
  Serial.println(" Error opening 'sensor.txt' " );
}
 void read_From_SD()
 {
   myFile = SD.open("sensor.txt");
  if (myFile) {
    Serial.println(" Values From SD Card :");

    while (myFile.available()) {
      Serial.write(myFile.read());  
    }
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  
 }
 }


