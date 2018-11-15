#include <Servo.h>
float vibrationValue = 0;
byte vibPin = 1;
Servo myservo;

void setup() {
    Serial.begin(9600);
    //pinMode(14,OUTPUT);
    myservo.attach(A6);
    // Initially the servo must be stopped 
    myservo.write(90);
}

void loop() {
        vibrationValue = checkVibration(vibPin);
        Serial.println(vibrationValue);
        if( vibrationValue > 1 )
            myservo.write(180);
        else
            myservo.write(0);
}


float checkVibration(byte vibPin)
{
    // Read Piezo ADC value in, and convert it to a voltage
    int piezoADC = analogRead(vibPin);
    float piezoV = piezoADC / 1023.0 * 5.0;
    return piezoV;
}

