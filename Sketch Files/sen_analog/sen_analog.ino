int sen=0;
int val=0;
void setup() {
Serial.begin(9600);
}

void loop() {
 val=analogRead(sen);
 
 Serial.println(val);
}
