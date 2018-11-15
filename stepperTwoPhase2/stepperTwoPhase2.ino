/*
 * Written by: Ahmad Saeed Mohammad Saeed
 * mail: ahmad._.saeed@outlook.com
 */
 
#define A        30                     // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire) 
#define A_bar    31                     // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire)
#define B        32                     // the pin connected to the wire B of the coil A (or to the H-bridge pin controlling the same wire)
#define B_bar    33                     // the pin connected to the wire B- of the coil A (or to the H-bridge pin controlling the same wire)
#define x       10                 // smaller values may make the motor produce more speed and less torque
#define inputPin 37
#define stepsPerRevolution 200         // you can the number of steps required to make a complete revolution in the data sheet of your motor

boolean currentState = false;
void setup() {
  pinMode(A, OUTPUT);
  pinMode(A_bar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(B_bar, OUTPUT);
}


void loop() {  

   if( digitalRead(inputPin) == HIGH ) {
    
      for (int i = 0; i < (stepsPerRevolution/4) ; i++) {
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delay (x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delay(x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delay(x);
    
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delay(x);
      }
      // Counter Direction 
      for (int i = 0; i < (stepsPerRevolution/4); i++) {
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delay(x);    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delay(x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delay(x);
    
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delay(x);
      }
  }
}

