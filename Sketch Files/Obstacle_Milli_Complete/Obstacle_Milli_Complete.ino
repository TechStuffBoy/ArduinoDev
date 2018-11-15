//Library For IR Remote 
#include <IRremote.h>

//initializers for ultrasonicSensor
int distance=0,duration=0;

// Pins for motors in motorShield
const byte m1p1 = 2;
const byte m1p2 = 3;
const byte m2p1 = 4;
const byte m2p2 = 5;
const byte En1 = 6;
const byte En2 = 7;

// Initializers For irReciever [ TSOP1738 ]
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
long int decCode;
decode_results results;
#define trigPin 9
#define echoPin 10

// For Timers 1 - 5
unsigned long currentMillis1;
unsigned long previousMillis1 = 0;
const int interval1 = 500;

unsigned long currentMillis2;
unsigned long previousMillis2 = 0;
const int interval2 = 500;

unsigned long currentMillis3;
unsigned long previousMillis3 = 0;
const int interval3 = 500;

unsigned long currentMillis4;
unsigned long previousMillis4 = 0;
const int interval4 = 500;

unsigned long currentMillis5;
unsigned long previousMillis5 = 0;
const int interval5 = 500;

unsigned int timer1Count = 0;
unsigned int timer1CountCheck = 0;

unsigned int timer2Count = 0;
unsigned int timer2CountCheck = 0;

unsigned int timer3Count = 0;
unsigned int timer3CountCheck = 0;

unsigned int timer4Count = 0;
unsigned int timer4CountCheck = 0;

unsigned int timer5Count = 0;
unsigned int timer5CountCheck = 0;

//Flags for timers
boolean timer1Flag = false;
boolean timer2Flag = false;
boolean timer3Flag = false;
boolean timer4Flag = false;
boolean timer5Flag = false;

void setup()
{
  pinMode(m1p1,OUTPUT);
  pinMode(m1p2,OUTPUT);
  pinMode(m2p1,OUTPUT);
  pinMode(m2p1,OUTPUT);
  pinMode(En1,OUTPUT);
  pinMode(En2,OUTPUT);
  Serial.begin(9600);
  Serial.println("startin...");
  pinMode(trigPin ,OUTPUT);
  pinMode(echoPin,INPUT);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  ultrasonic(); // to get the distance
  //starting the timers. should be in void loop
  currentMillis1 = millis();
  currentMillis2 = millis();
  currentMillis3 = millis();
  currentMillis4 = millis();
  currentMillis5 = millis();

  // Checking and Performonging this loop if timer1Flag = true
  if( timer1Flag == true){
    timer1();
    if ( timer1Count != timer1CountCheck ){
      timer1CountCheck = 0;
      timer1Count = 0;
      Stop();
      timer1Flag = false;
    }
  }

  // Checking and Performonging this loop if timer2Flag = true
  if( timer2Flag == true){
    timer2();
    if ( timer2Count != timer2CountCheck ){
      timer2CountCheck = 0;
      timer2Count = 0;
      Stop();
      timer2Flag = false;
    }
  }

  // Checking and Performonging this loop if timer3Flag = true
  if( timer3Flag == true){
    timer3();
    if ( timer3Count != timer3CountCheck ){
      timer3CountCheck = 0;
      timer3Count = 0;
      Stop();
      timer3Flag = false;
    }
  }

  // Checking and Performonging this loop if timer4Flag = true
  if( timer4Flag == true){
    timer4();
    if ( timer4Count != timer4CountCheck ){
      timer4CountCheck = 0;
      timer4Count = 0;
      Stop();
      timer4Flag = false;
    }
  }

  // Checking and Performonging this loop if timer5Flag = true
  if( timer5Flag == true){
    timer5();
    if ( timer5Count != timer5CountCheck ){
      timer5CountCheck = 0;
      timer5Count = 0;
      Stop();
      timer5Flag = false;
    }
  }

  // Here only we check oour Button press of the remote.
  // Note : You can not use any remote. if so it should be configured in if loop
  if (irrecv.decode(&results)) {
    
    Serial.println(results.value, HEX);
    decCode = results.value;

    // Forward = Button 2
    if (decCode == 0x2D607981){
      previousMillis1 = currentMillis1;
      Serial.println("Forward");
      Forward();
      timer1Flag = true;
     }
     // Backward = Button 8
    else if (decCode == 0xC264BDB9){
      previousMillis2 = currentMillis2;
      Serial.println("Backward");
      Backward();
      timer2Flag = true;
    }
    // Left = Button 4
    else if (decCode == 0x92DA21E3){
      previousMillis3 = currentMillis3;
      Serial.println("Left");
      Left();
      timer3Flag = true;
    }
    // Right = Button 6
    else if (decCode == 0x40876B7F){
      previousMillis4 = currentMillis4;
      Serial.println("Right");
      Right();
      timer4Flag = true;
    }
    // Rotate = Button 5
    else if (decCode == 0x9F8ECFD5){
      previousMillis5 = currentMillis5;
      Serial.println("Rotate");
      Rotate();
      timer5Flag = true;
     }
    irrecv.resume(); // Receive the next value
  }
  delay(10); // giving some time for the irReciever to read another button value
 }

// Rotating the Motor Forward
void Forward(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , HIGH);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}

// Rotating the Motor Backward
void Backward(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , HIGH);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , HIGH);
  digitalWrite( m2p2 , LOW);
}
// Rotating the Motor Left
void Left(){
  digitalWrite( En1 , LOW);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}

// Rotating the Motor Right
void Right(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , LOW);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , HIGH);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , LOW);
}

// Rotating the Motor Rotate
void Rotate(){
  digitalWrite( En1 , HIGH);
  digitalWrite( En2 , HIGH);
  digitalWrite( m1p1 , HIGH);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , HIGH);
}
// Rotating the Motor Stop
void Stop(){
  digitalWrite( En1 , LOW);
  digitalWrite( En2 , LOW);
  digitalWrite( m1p1 , LOW);
  digitalWrite( m1p2 , LOW);
  digitalWrite( m2p1 , LOW);
  digitalWrite( m2p2 , LOW);
  }
// Function for getting the Distance
void ultrasonic(){
    digitalWrite(trigPin ,HIGH);
    delayMicroseconds(1000);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2)/29.1;
    Serial.print(distance);
    Serial.println("Cm");
    if( distance <= 12 ){
      Serial.println("Motors Stopped");
      Stop();
      delay(100);
      Backward();
    }
  delay(10);
}

// Timer1 Function
void timer1(){
      if( currentMillis1 - previousMillis1 >= interval1 ){
            previousMillis1 = currentMillis1;
            Serial.println("  I am in First Loop ");
             timer1CountCheck = timer1Count++;
      }
}

// Timer2 Function
void timer2(){
      if( currentMillis2 - previousMillis2 >= interval2 ){
            previousMillis2 = currentMillis2;
            Serial.println("  I am in Second Loop ");
            timer2CountCheck = timer2Count++;
      }
}

// Timer3 Function
void timer3(){
      if( currentMillis3 - previousMillis3 >= interval3 ){
            previousMillis3 = currentMillis3;
            Serial.println("  I am in Third Loop ");
             timer3CountCheck = timer3Count++;
      }
}

// Timer4 Function
void timer4(){
      if( currentMillis4 - previousMillis4 >= interval4 ){
            previousMillis4 = currentMillis4;
            Serial.println("  I am in Fourth Loop ");
            timer4CountCheck = timer4Count++;
      }
}

// Timer5 Function
void timer5(){
      if( currentMillis5 - previousMillis5 >= interval5 ){
            previousMillis5 = currentMillis5;
            Serial.println("  I am in Fifth Loop ");
            timer5CountCheck = timer5Count++;
      }
}
