#include<Servo.h>

char incomingByte = 0;    // for incoming serial data
int servoPin = A3;
boolean forward = false;
boolean reverse = false;
boolean stop = false;

Servo trolly;
void setup() {
    Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
    trolly.attach(servoPin);
     stopServo(trolly);
}

void loop() {
     
     //trolly.write(0);
    
    // send data only when you receive data:
    if (Serial.available() > 0 ) {
        // read the incoming byte:
        incomingByte = Serial.read();
        if(incomingByte == '1'){
            Serial.println("Forward");
            forward = true;
            reverse = false;
            stop = false;
        } else if(incomingByte == '2'){
             Serial.println("Reverse");
             forward = false;
             reverse = true;
             stop = false;
        } else if(incomingByte == '3'){
             forward = false;
             reverse = false;
             stop = true;
            }
    }
    if(forward)
        fwdServo(trolly);
    else if(reverse)
        revServo(trolly);
    else if(stop)
        stopServo(trolly);
        delay(10);
        
        /*
        fwdServo(trolly);
        delay(4500);
        revServo(trolly);
        delay(4500);
        */
        
}

void initServo()
{
    trolly.attach(servoPin); // servo attached to Analog Pin 3
    trolly.write(90); // Initially the servo must be stopped 
}
void stopServo(Servo servo)
{
    Serial.println("Stopped");
    servo.detach(); // detach servo
}
void fwdServo(Servo servo)
{
    Serial.println("Running Forward...");
    servo.attach(servoPin); // Attach servo
    servo.write(72);//rotate the Servo forward, 77,73,72
}

void revServo(Servo servo)
{
    Serial.println("Running Reverse...");
    servo.attach(servoPin); //Attach servo
    servo.write(95); //rotate the Servo reverse , 94, 94,95
}
