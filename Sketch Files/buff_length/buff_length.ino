int mySensVals[6] = {2, 4, -8, 3, 2};
char message[] = "hello";
void setup() {
    Serial.begin(9600);
    int i;
    int j;
    int b;
    b=sizeof(message);
    Serial.println(b);
    for (i = 0; i < 5; i = i + 1) {
      Serial.println(mySensVals[i]);
    }
     for (j= 0; j < 6; j++) {
      Serial.println(message[j]);
      b=sizeof(message);
      Serial.println(b);
    }
}

void loop() {

}
