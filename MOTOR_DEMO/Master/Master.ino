#include<Wire.h>

void setup()
{
    Serial.begin(9600);
    Serial.println("From Master");
    Wire.begin();
}

void loop()
{
    while(1){
        
        Wire.beginTransmission(8);
        Serial.println("One");
        Wire.write("One");
        Wire.endTransmission();
    
        delay(100);
    }
    
}

