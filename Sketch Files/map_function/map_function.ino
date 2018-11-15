int x=254;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  x=map(x,255,0,0,255);
  Serial.println(x);

}

void loop() {
  // put your main code here, to run repeatedly:

}
