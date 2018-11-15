#include <Servo.h>
Servo myservo;

void setup() {
    myservo.attach(A2);
    // Initially the servo must be stopped 
}

void loop() 
{
   // myservo.write(0);
        // myservo.writeMicroseconds(2300);
         //delay(2000);
         //myservo.write(180);
         // myservo.writeMicroseconds(1500);
         //delay(2000);
  //
//  <-----Final---------->
         myservo.write(0);
         delay(2000);
          myservo.write(180);
         delay(2000);
//          myservo.write(65);
//         delay(4000);
       
         //myservo.write(100);
         //delay(4000);
//
//         myservo.write(0);
//         delay(2000);
//         myservo.write(180);
//         delay(2000);
        // <------Final------->

       /*
        myservo.attach(A0);
        myservo.write(0);
        delay(2000);
        myservo.detach();
        delay(2000);
        myservo.attach(A0);
        myservo.write(180);
        delay(2000);
        myservo.detach();
        delay(2000);
        */


//
//    for(int i=0;i<50;i++){
//        myservo.attach(A0);
//        myservo.write(0);
//        delay(100);
//        myservo.detach();
//        delay(100);
//        myservo.attach(A0);
//        myservo.write(0);
//        delay(100);
//        myservo.detach();
//        delay(100);
//   }

/*
 for(int i=0;i<50;i++){
        myservo.write(0);
        delay(10);
        myservo.write(90);
        delay(10);
        myservo.write(0);
        delay(10);
        myservo.write(90);
        delay(10);
    }
 */
    
//        myservo.write(0);
//        delay(2000);
//        myservo.write(60);
//        delay(2000);
//        delay(4);
//        myservo.write(90);
//        delay(4);
        
    /*
    // Start turning clockwise
    for(int i=0;i<50;i++){
        myservo.write(0);
        delay(10);
        myservo.write(90);
        delay(10);
        myservo.write(0);
        delay(10);
        myservo.write(90);
        delay(10);
    }
    myservo.write(90);
    delay(2000);

     for(int i=0;i<50;i++){
        myservo.write(180);
        delay(10);
        myservo.write(90);
        delay(10);
        myservo.write(180);
        delay(10);
    }

    myservo.write(90);
    delay(2000);
    */
}
