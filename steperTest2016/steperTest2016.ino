/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 int speed = 200;
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 int delayy=700;
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(53);// attaches the servo on pin 9 to the servo object 
  myservo.write(0);
  delay(2000);
}
 
void loop() 
{ 
    
   
    
/*
    myservo.write(155);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(140);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(125);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(105);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(90);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(75);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(60);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(45);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(30);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(15);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(0);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    



    
    myservo.write(0);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(15);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(30);              // tell servo to go to position in variable 'pos' 
    delay(speed);  
    myservo.write(45);              // tell servo to go to position in variable 'pos' 
   delay(speed);  
    myservo.write(60);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(75);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(90);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(105);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
     myservo.write(125);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(140);              // tell servo to go to position in variable 'pos' 
    delay(speed); 
    myservo.write(155);              // tell servo to go to position in variable 'pos' 
    delay(speed); 

   

*/
    


  for(pos = 0; pos <= 180; pos += 5) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                     // waits 15ms for the servo to reach the position 
  }
  delay(delayy); 
  for(pos = 180; pos>=0; pos-=5)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
  delay(2000);

  
}



