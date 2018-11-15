#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
byte potOnePin = 0;// Analog 0 port
byte potTwoPin = 1;
byte tempPin = 2;
byte vibPin = 6;
int potOneVal = 0;
int potTwoVal = 0;
int tempC = 0;
int vibrationValue = 0;
byte data[4]={0,0,0,0};
 
void setup() {
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
}
 
void loop() {
    delay(10);
    potOneVal = readPot(potOnePin);
    potTwoVal =  readPot(potTwoPin);
    tempC = readTemp(tempPin);
    vibrationValue = readVib(vibPin);
 
}
 
// callback for received data
void receiveData(int byteCount){
 
 while(Wire.available()) {
  number = Wire.read();
 
  if (number == 1){
   if (state == 0){
    digitalWrite(13, HIGH); // set the LED on
    state = 1;
   } else{
    digitalWrite(13, LOW); // set the LED off
    state = 0;
   }
  }
 
  if(number==2) {
   number = 88;
   }
 }
}
 
// callback for sending data
void sendData(){
 
 data[0] = (byte)potOneVal;
 data[1]= (byte)potTwoVal;
 data[2] = (byte)tempC;
 data[3] = (byte)vibrationValue;
 Wire.write(data,4);
 }

int readTemp(byte tempPin)
{
     // put your main code here, to run repeatedly:
  float rawAdcVal = 0;
  float voltage = 0;
  float volInMilli = 0;
  float celcius = 0;
  float farenheit = 0;
  for(int i=1;i<200;i++){
    rawAdcVal += analogRead(tempPin);
    delay(5);
  }
  rawAdcVal /= 200.0;
  voltage = (5 * rawAdcVal) / 1024;
  //voltage = (aref_voltage * rawAdcVal) / 1024; //for 3.3v
  volInMilli = voltage * 1000;
  celcius = (volInMilli - 500) / 10; // 500 is offset for tmp36,10 is 10mv precision.
  farenheit = (celcius * 9.0 / 5.0) + 32.0;
  int val = (int)celcius;
  Serial.print("celcius = ");
  Serial.println(val);
  return val;
}

 int readVib(byte vibPin)
 {
    // Read Piezo ADC value in, and convert it to a voltage
  float piezoADC = analogRead(vibPin);
  float piezoV = piezoADC / 1023.0 * 5.0;
  int value = (int)piezoV*10;
  Serial.print("VibValue = ");
  Serial.println(value);
  return value;
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
/*
 * int readPot(byte potPin){
    float rawV = analogRead(potPin);
    float potval= rawV/1023.0;
    //int val = (int)potval;
    Serial.print("PotValue at Analog");
    Serial.print(potPin);
    Serial.print("-->");
    Serial.println((int)potval);
    return potval;
 }

 */

