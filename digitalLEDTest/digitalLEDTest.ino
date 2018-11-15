#define A        32                     // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire) 
#define A_bar    23                     // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire)
#define B        24                     // the pin connected to the wire B of the coil A (or to the H-bridge pin controlling the same wire)
#define B_bar    25                     // the pin connected to the wire B- of the coil A (or to the H-bridge pin controlling the same wire)

void setup() {
  pinMode(A, OUTPUT);
   pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
     pinMode(36, OUTPUT);
  pinMode(A_bar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(B_bar, OUTPUT);
  Serial.begin(9600);
}
void loop() 
{  
     Serial.println("jjjjjjj");
    /*
    digitalWrite(A, HIGH);
    digitalWrite(A_bar, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_bar, HIGH);
   
    delayMicroseconds (1500);
    
    digitalWrite(A, HIGH);
    digitalWrite(A_bar, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(B_bar, HIGH);

    delayMicroseconds (1500);
    */
    digitalWrite(22, HIGH);
     digitalWrite(23, HIGH);
      digitalWrite(34, HIGH);
       digitalWrite(35, HIGH);
        digitalWrite(36, HIGH);

    
}
