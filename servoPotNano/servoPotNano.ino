#include <Servo.h>

int pot1 = 6;
int val = 0;
Servo myservo;

void setup() {
    myservo.attach(19);
    // Initially the servo must be stopped 
    myservo.write(90);
}

void loop() {
        val = analogRead(A6);
        if(val > 500 ) {
            myservo.write(0);
        } else if(  val < 500 ){
            myservo.write(180);
        }
        
}
