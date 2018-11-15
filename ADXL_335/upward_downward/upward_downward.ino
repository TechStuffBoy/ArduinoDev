unsigned int unitX = 0;
unsigned int unitY = 0;
unsigned int unitZ = 0;

int xMin = 529;
int xMAx = 526;
int yMin = 532;
int yMax = 532;
int zMin = 425;
int zMax = 630;
float Xgram = 0.0;
float Ygram = 0.0;

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

//    for(int i=0 ; i<50; i++){
//       unitX += analogRead(xPin);
//       delay(5); 
//    }
     for(int i=0 ; i<50; i++){
       unitY += analogRead(yPin);
       delay(5); 
    }
    //unitX = unitX/50;
    unitY = unitY/50;
   // Xgram = (unitX - 527.0)*0.330;
    Ygram = (unitY - 527.0)*0.330;
//    Serial.print("Xgram :");
//    Serial.print(Xgram);
    Serial.print("  Ygram :");
    Serial.println(Ygram);
    //unitX = 0;
    unitY = 0;
    delay(100);

}
