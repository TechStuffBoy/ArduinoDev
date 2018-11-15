unsigned int unitX = 0;
unsigned int unitY = 0;
unsigned int unitZ = 0;
float Xgram = 0;
float Ygram = 0;

byte xPin = 0;
byte yPin = 1;
byte zPin = 2;

void setup() {
    analogReference(EXTERNAL);
    Serial.begin(9600);
}

void loop() {
    unitX = analogRead(xPin);
    unitY = analogRead(yPin);
    unitZ = analogRead(zPin);

    for(int i=0 ; i<50; i++){
       unitY += analogRead(yPin);
       delay(5); 
    }
    unitY = unitY/50;
    Ygram = (unitY - 527.0)*0.330;
    Serial.print("ygram :");
    Serial.println(Ygram);
    unitY = 0;

    delay(100);

}
