#include <Servo.h>
Servo myservo;

void setup() {
    myservo.attach(A0);
    Serial.begin(9600);
    // Initially the servo must be stopped 
}

void loop()
{
    Serial.println("Servo Degree 73");
    myservo.write(73);
    delay(4000);

    //95
    Serial.println("Servo Degree 91");
    myservo.write(92);
    delay(4000);
    
    /*
    //73
    Serial.println("Servo Degree 91");
    myservo.write(91);
    delay(4000);

    //95
    Serial.println("Servo Degree 92");
    myservo.write(92);
    delay(4000);

    Serial.println("Servo Degree 93");
    myservo.write(93);
    delay(4000);

    Serial.println("Servo Degree 94");
    myservo.write(94);
    delay(4000);
    */
    
    /*
    Serial.println("Servo Degree 0");
    myservo.write(0);
    delay(1500);

    Serial.println("Servo Degree 10");
    myservo.write(10);
    delay(1500);

    Serial.println("Servo Degree 20");
    myservo.write(20);
    delay(1500);

    Serial.println("Servo Degree 30");
    myservo.write(30);
    delay(1500);
    
    Serial.println("Servo Degree 40");
    myservo.write(40);
    delay(1500);

    Serial.println("Servo Degree 50");
    myservo.write(50);
    delay(1500);

    Serial.println("Servo Degree 60");
    myservo.write(60);
    delay(1500);

    Serial.println("Servo Degree 70");
    myservo.write(70);
    delay(1500);

    Serial.println("Servo Degree 80");
    myservo.write(80);
    delay(1500);

    Serial.println("Servo Degree 90");
    myservo.write(90);
    delay(1500);

    Serial.println("Servo Degree 100");
    myservo.write(100);
    delay(1500);

    Serial.println("Servo Degree 110");
    myservo.write(110);
    delay(1500);

    Serial.println("Servo Degree 120");
    myservo.write(120);
    delay(1500);

    Serial.println("Servo Degree 130");
    myservo.write(130);
    delay(1500);

    Serial.println("Servo Degree 140");
    myservo.write(140);
    delay(1500);

    Serial.println("Servo Degree 150");
    myservo.write(150);
    delay(1500);

    Serial.println("Servo Degree 160");
    myservo.write(160);
    delay(1500);

    Serial.println("Servo Degree 170");
    myservo.write(170);
    delay(1500);

    Serial.println("Servo Degree 180");
    myservo.write(180);
    delay(1500);
    */
}

