byte clockPin = 3; // controlling speed
byte directionPin = 4; // High->Fwd,Low->Bwd
byte enablePin =5; // High-> Enable,Low->Disable
byte halfOrFull =6; // High->halfStep,Low->fullStep
int timeOn = 100;
int timeOff =100;

void setup(){
  Serial.begin(9600);
  digitalWrite(enablePin, HIGH );
}
void loop(){

  //analogWrite(clockPin ,20);

   digitalWrite(clockPin, HIGH);
   delay(1);
  digitalWrite(clockPin, LOW);
  delay(1);
  FwdHalfStep();
//  delay(5000);
//  FwdFullStep();
//  delay(5000);
//  ReverseHalfStep();
//  delay(5000);
//  ReverseFullStep();
//  delay(5000);
  }

void FwdHalfStep(){
   //ClockFunction();
   Serial.println("loop1");
   digitalWrite(directionPin , HIGH );
   digitalWrite(halfOrFull , HIGH );
}

void FwdFullStep(){
  //ClockFunction();
  Serial.println("loop2");
  digitalWrite(directionPin , HIGH );
  digitalWrite(halfOrFull , LOW );
  
}
void ReverseHalfStep(){
  //ClockFunction();
  Serial.println("loop3");
  digitalWrite(directionPin , LOW );
  digitalWrite(halfOrFull , HIGH );
}

void ReverseFullStep(){
  //ClockFunction();
  Serial.println("loop5");
  digitalWrite(directionPin , LOW );
  digitalWrite(halfOrFull , LOW );
}
void ClockFunction(){
//  digitalWrite(clockPin, HIGH);
//   delayMicroseconds(timeOn);
//  digitalWrite(clockPin, LOW);
//   delayMicroseconds(timeOff);
   analogWrite(clockPin , 127);
}

