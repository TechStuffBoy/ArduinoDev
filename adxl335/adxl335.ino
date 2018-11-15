
unsigned char unitX = 0;
unsigned char unitY = 0;
unsigned char unitZ = 0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;


void setup() {
     analogReference(EXTERNAL);
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    unitX = analogRead(xPin);
    unitY = analogRead(yPin);
    unitZ = analogRead(zPin);

    Serial.print("zAxis :");
    Serial.println(unitZ);
//    Serial.print(" yAxis :");
//    Serial.print(unitY);
//    Serial.print(" zAxis :");
//    Serial.println(unitZ);

    delay(150);
    
}
