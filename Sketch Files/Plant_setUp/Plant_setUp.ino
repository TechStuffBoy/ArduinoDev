#include <rgb_lcd.h>
#include<Wire.h>
//Object for Grove_Lcd
rgb_lcd lcd;
// Defining RGB in Lcd
const int colorR= 128;
const int colorG=125;
const int colorB=33;

// Conveyor Belt Initialize
int belt=2;
//int belt2=3;

// Tile Pushing arm Initialize
int arrow=3;
//int arrow2=5;

// Rejection Gun Initialize
int gun=4;
//int gun2=7;

// In detect Sensor Initialize
int in_sen=5;

//Bug Detect sensor Initialize
int bug_sen=6;

//Raspberry Camera Fault
int Raspberry_cam=7;

//Variable for Button
int Raspberry_cam_val=0;

int in_sen_val=0;
int bug_sen_val=0;

//In Detect sensor counter
int in_count=0;

//Bug Detect sensor counter
int bug_count=0;

// Final goodone sensor counter
int Good_ones=0;

void setup()
{
 //Motor pins are set to OUTPUT
 pinMode(belt,OUTPUT);
// pinMode(belt2,OUTPUT);
 pinMode(arrow,OUTPUT);
// pinMode(arrow2,OUTPUT);
 pinMode(gun,OUTPUT);
// pinMode(gun2,OUTPUT);
 
 //Sensor Pin are Set to INPUT
 pinMode(in_sen,INPUT);
 pinMode(bug_sen,INPUT);
 pinMode(Raspberry_cam,INPUT);
 
 //Serial Monitor Initialize
 Serial.begin(9600);
 
 //Initialize Lcd
 lcd.begin(16,2);
 lcd.setRGB(colorR, colorG, colorB);
 
 // printing Tiles Entering in count
 lcd.setCursor(0,0);
 lcd.print(" TilesIn:");
 
 // printing Good Tiles count
 lcd.setCursor(0,1);
 lcd.print("GudTiles:");

 //Initially Conveyor Belt and Arm is ON!!!
 digitalWrite(belt,HIGH);
 //digitalWrite(belt2,LOW);
 digitalWrite(arrow,HIGH);
 //digitalWrite(arrow2,LOW);

 // FireGun should be in OFF position
 digitalWrite(gun,LOW);
 //digitalWrite(gun2,LOW);
 }
 
void loop()
{
// Reading the in_sensor  
in_sen_val=digitalRead(in_sen);

// Initially Reading Raspberry_cam_val 
//Raspberry_cam_val= digitalRead(Raspberry_cam);

// Function for InSensor
while(in_sen_val != 0)
{
  in_sen_val=digitalRead(in_sen);
  while(!in_sen_val)
  {
    // Increase the in_Counter
    in_count++;
    lcd.setCursor(10,0);
    lcd.print(in_count);
    delay(5);
    lcd.setCursor(10,0);
    lcd.print(" ");
    Serial.print("in_count :");
    Serial.println(in_count);
    break;
  }
} // Read inSensor Function Ends

// Now checking whether Raspberry pi camera detects wrong ones or not
Raspberry_cam_val= digitalRead(Raspberry_cam);

// If fault Tile is Found
if( Raspberry_cam_val )
{
 // Checking Bug sensor Value
 bug_sen_val= digitalRead( bug_sen);
 // If bug sensor is HIGH ==> object Found
  while(bug_sen_val != 0 )
 {
 // We have to count that Tile as Wrong one
   bug_sen_val= digitalRead( bug_sen);
   while(!bug_sen_val)
  {
    // Increase the bug_Counter
    bug_count++;
    Serial.print("bug_count :");
    Serial.println(bug_count);

    
    goodOne_check();
    break;
  } 
}
 // Fire the Gun to throw it cout 
    digitalWrite(gun,HIGH);
    //digitalWrite(gun2,LOW);
    delay(100);
    //Switch off the Gun
    digitalWrite(gun,LOW);
    //digitalWrite(gun2,LOW);
 }
}

void goodOne_check()
{
  Good_ones=in_count-bug_count;
  lcd.setCursor(10,1);
  lcd.print(Good_ones);
  delay(5);
  lcd.setCursor(10,1);
  lcd.print(" ");
  Serial.print("Good_ones :");
  Serial.println(Good_ones);
}
