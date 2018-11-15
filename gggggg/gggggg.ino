byte b = 0b10000000;
int bitNb = 7; // Third bit from the right


void setup() {
    Serial.begin(9600);
  // put your setup code here, to run once:
 int x = b & (1 << bitNb);
if (x == 0) Serial.println("0");
else Serial.println("1");
}

void loop() {
  // put your main code here, to run repeatedly:

}
