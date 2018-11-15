int a = 0;
 void setup()
 {
    Serial.begin(9600);
 }
void loop()
{
    int t = a;
    a++;
    if( t != a) {
        Serial.println("a not equal to a");
    } 
    delay(5000);
}

