char buffer[200];
int time=7;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sprintf (buffer, "you have %d hours to come to me",time);
Serial.println(buffer);

}
