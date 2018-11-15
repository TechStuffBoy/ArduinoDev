#include <Stepper.h>
int clockwise= 6;
int anticlockwise= 7;
int steps =200;
int enable=8;
Stepper motor(steps, 2,3,4,5);
void setup() {
pinMode(clockwise,INPUT);
pinMode(anticlockwise,INPUT);
pinMode(enable,OUTPUT);
Serial.begin(9600);
}
void loop() {
int Speed = analogRead(A0);
int RPM = map(Speed, 0, 1023, 0, 100);
int forward = digitalRead(clockwise);
int reverse = digitalRead(anticlockwise);
digitalWrite(enable,LOW);
if(forward == 1 && reverse == 0 && RPM > 5){
digitalWrite(enable,HIGH);
motor.step(1);
motor.setSpeed(RPM);
delay(10);
}
if(reverse == 1 && forward == 0  && RPM > 5){
digitalWrite(enable,HIGH);
motor.step(-1);
motor.setSpeed(RPM);
delay(10);
}
delay(5);
Serial.println("RPM is :");
Serial.println(RPM );
}
