#include<Wire.h>
#define SLAVE_ADDRESS 0x04

const byte trigPin = 9;
const byte echoPin = 10;

long duration;
int distance;

byte data[2];

void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendAllData);
}

void loop()
{
    int distanceInCm = 0;
    distanceInCm = calculateDistance();
//    Serial.print("Distance in cm :");
//    Serial.print(distanceInCm);
//    Serial.println(" Cm");
    delay(1000);
}

void sendAllData()
{
    int distanceInCm = 0;
    distanceInCm = calculateDistance();
    data[0] = distanceInCm&0x7F;
    data[1] = (distanceInCm & 0xFF80) >> 7;
    //Parse -> (data[1] << 7 ) | data[0].
    Wire.write(data,2);
}
int calculateDistance()
{
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculating the distance
    distance= duration*0.017;
    return distance;
}

