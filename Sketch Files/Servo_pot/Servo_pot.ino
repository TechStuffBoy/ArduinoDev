
// Controlling a servo position using a potentiometer (variable resistor) 
// with the option of recording the movement, playing it in reverse and in original order.
// Adi Soffer 19.3.12

#include <Servo.h> 

Servo myservo;               // create servo object to control a servo 
char buffer [5];              //variable to hold keystrokes
int potpin = 0;                // analog pin used to connect the potentiometer
int val;                          // variable to read the value from the analog pin 
int servoPos[300];           //create array 300 values for servo position
int incomingByte;            //declare variable to hold incoming letter

void setup() {
  Serial.begin(9600);
  Serial.flush();
  myservo.attach(3);           // attaches the servo on pin 3 to the servo object 
}

void loop (){
if (Serial.available( ) > 0) 
{
incomingByte = Serial.read ( );
if (incomingByte == 'F') 
{
   Serial.println ("Free mode");
   while (incomingByte == 'F') 
{
   val = analogRead (potpin);
   val = map(val, 0, 1023, 0, 179);
   myservo.write (val);
   delay (10);
   if (Serial.available ( ) > 0)
{
     incomingByte = Serial.read ( );
     if (incomingByte == 'R') 
  {
       Serial.println ("Record inside");                  // if so - prints record inside for checkup
       delay(15);
       for (int i=0;i<299;i++)                              //for loop to store 300 values of pot  
    {
       val = analogRead(potpin);                         // reads the value of the potentiometer (value between 0 and 1023) 
       val = map(val, 0, 1023, 0, 179);                // scale it to use it with the servo (value between 0 and 180) 
       myservo.write(val);                                 // sets the servo position according to the scaled value 
       servoPos [i]=val;                                     // stores val in array "servoPos
       delay(15);                                              // waits for the servo to get there
       Serial.println(val);                                   // print values for checking
    }    
  } 
    else if (incomingByte == 'H') 
{
     Serial.println ("Home inside");
     //delay(15);
     for (int i=299;i>0;i--) 
  {
       myservo.write (servoPos[i]);           //reads values from array in reverse to servo
       delay(15);
       Serial.println (servoPos[i]);
  }
 }   
    else if (incomingByte = 'P') 
 {
      Serial.println ("Playback inside");       // if so prints for checkup
      //delay(15);
      for (int i=0;i<299;i++) 
   {
        myservo.write (servoPos[i]);          //reads values from array in original order to servo
        delay (15);
        Serial.println (servoPos[i]);
    } 
  }    
}  
}
}  
   else if (incomingByte == 'R')                // checks if input was capital R
 {                       
     Serial.println ("Record");                   // if so - prints record
     delay(15);
     for (int i=0;i<299;i++)                      //for loop to store 300 values of pot  
  {
       val = analogRead(potpin);               // reads the value of the potentiometer (value between 0 and 1023) 
       val = map(val, 0, 1023, 0, 179);      // scale it to use it with the servo (value between 0 and 180) 
       myservo.write(val);                       // sets the servo position according to the scaled value 
       servoPos [i]=val;                           // stores val in array "servoPos
       delay(15);                                    // waits for the servo to get there
       Serial.println(val);                         // print values for checking
  }  
 }

   else if (incomingByte == 'H') 
{
     Serial.println ("Home");
     //delay(15);
     for (int i=299;i>0;i--) 
  {
       myservo.write (servoPos[i]);           //reads values from array in reverse to servo
       delay(15);
       Serial.println (servoPos[i]);
  }
 }

   else if (incomingByte = 'P') 
 {
      Serial.println ("Playback");
      //delay(15);
      for (int i=0;i<299;i++) 
   {
        myservo.write (servoPos[i]);          //reads values from array in original order to servo
        delay (15);
        Serial.println (servoPos[i]);
    } 
  }
}
}
