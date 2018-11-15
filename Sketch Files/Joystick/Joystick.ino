int xpin=0;
int ypin=1;
int xval=0;
int swt=3;
int swtval=0;
int yval=0;
void setup(){
  Serial.begin(9600);
  pinMode(swt,INPUT);
}
void loop(){
  swtval=digitalRead(swt);
  xval=analogRead(xpin);
  yval=analogRead(ypin);
  Serial.print(xval);
  Serial.print(" : ");
  Serial.print(yval);
  Serial.print(" : ");
  swtval=digitalRead(swt);
  Serial.println(swtval);
  delay(300);
}

