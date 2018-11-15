#define trigPin 9
#define echoPin 10
#include<Servo.h>

byte m1p1 = 2; // in1
byte m1p2 = 3; //in2
byte m2p1 = 4; 
byte m2p2 = 5;
byte En1 = 6;
byte En2 = 7;

int distance,duration;
Servo myServo;
boolean checkLeft = false;
boolean checkRight = false; 
void setup()
{
  Serial.begin(9600);
  myServo.attach(12);
  pinMode(m1p1,OUTPUT);
  pinMode(m1p2,OUTPUT);
  pinMode(m2p1,OUTPUT);
  pinMode(m2p1,OUTPUT);
  pinMode(En1,OUTPUT);
  pinMode(En2,OUTPUT);
  Serial.println("startin...");
  checkLeft = true;
  pinMode(trigPin ,OUTPUT);
  pinMode(echoPin,INPUT);
}
void loop()
{
  
      for(int i=15;i<=165;i++){  
          myServo.write(i);
          delay(30);
          distance = calculateDistance();
          Serial.print(i);
          Serial.print(",");
          Serial.print(distance);
          Serial.print(".");
          while( distance <= 10 ){
              Stop();
              delay(100);
              Backward();
              delay(100);
              Right();
              break;
              
          }
      }
 
    for(int i=165;i>15;i--){  
        myServo.write(i);
        delay(30);
        distance = calculateDistance();
        Serial.print(i);
        Serial.print(",");
        Serial.print(distance);
        Serial.print(".");
        while( distance <= 10 ){
              Stop();
              delay(100);
              Backward();
              delay(100);
              Right();
              break;
        }
    }
}

int calculateDistance()
{
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2);
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH); 
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
      distance= duration*0.034/2;
      return distance;
} 

void Forward(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , HIGH);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}

void Backward(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , HIGH);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , HIGH);
  digitalWrite( m2p2 , LOW);
}
void Left(){
  digitalWrite( En1 , LOW);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}

void Right(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , LOW);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , HIGH);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , LOW);
}

void Rotate(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , HIGH);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}
void Stop(){
  digitalWrite( En1 , LOW);
  digitalWrite( En2 , LOW);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , LOW);
  }

