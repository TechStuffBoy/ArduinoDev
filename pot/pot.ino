void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    int h = readPot(1);
    Serial.println(h);
}

int readPot(byte potPin){
    float rawV = analogRead(potPin);
    float potval= (rawV/1023.0)*127.0;
     int val = (int)potval;
    Serial.print("PotValue at Analog");
    Serial.print(potPin);
    Serial.print("-->");
    Serial.println((int)potval);
    return (int)potval;
 }

