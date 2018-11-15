#include <Servo.h>
Servo myservo;

void setup() {
    myservo.attach(A3);
    // Initially the servo must be stopped 
}

void loop() {

        myservo.write(0);
        delay(2000);
        myservo.write(90);
        delay(1500);
        myservo.write(180);
        delay(2000);
        myservo.write(90);
        delay(1500);




    
    /*
    // Start turning clockwise
    for(int i=0;i<50;i++){
        myservo.write(0);
        delay(10);
        myservo.write(90);
        delay(10);
        myservo.write(0);
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
