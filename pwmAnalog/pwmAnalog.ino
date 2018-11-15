int pwmPin = 3;
void setup() {
  // put your setup code here, to run once:
    
}

void loop() {
  // put your main code here, to run repeatedly:
    for(int i=50;i<255;i++){
        analogWrite(pwmPin,i);
        delay(25);
    }
    for(int i=255;i>50;i--){
        analogWrite(pwmPin,i);
        delay(25);
    }
    
}
