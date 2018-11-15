byte a = 126;
byte b = 0b00000000;

int BYTE = 380;
byte byte1 = 0;
byte byte2 = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    b = a >> 1;
    Serial.println(b);
    b = a << 1;
    Serial.println(b);

    byte1 = BYTE >> 8;
    byte2 = BYTE & 255;
    int retainByte = (byte1 << 8 ) | byte2;
    Serial.println(byte1);
    Serial.println(byte2);
    Serial.println(retainByte);
    while(1);
}
