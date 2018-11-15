#include <Servo.h>
Servo myservo;

void setup() {
    myservo.attach(A3);
    // Initially the servo must be stopped 
}

void loop() {
    
        myservo.attach(A3);
        myservo.write(0);
        delay(2000);
        
        myservo.detach();
        delay(2000);
        
        myservo.attach(A3);
        myservo.write(180);
        delay(2000);
        
        myservo.detach();
        delay(2000);

}
