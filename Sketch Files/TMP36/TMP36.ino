int senPin=0;
int voltage=0;
int degreesC=0;
int degreesF=0;

void setup(){
  Serial.begin(9600);
  
}
void loop(){
 voltage=analogRead(senPin);
 Serial.print("voltage :");
 Serial.print(voltage);
 Serial.print(" ' ");
 degreesC = (voltage - 0.5) * 100.0;
 degreesF = degreesC * (9.0/5.0) + 32.0;
 Serial.print("degreesC :");
 Serial.print(degreesC);
 Serial.print(" ' ");
 Serial.print("degreesF :");
 Serial.println(degreesF);
 delay(1000);
}
