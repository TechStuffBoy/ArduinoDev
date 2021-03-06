#include <Wire.h>
 
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
byte tmpPin = 0;// Analog 0 port
int tempC = 0;
byte data[3]={0,0,0};
 
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
 delay(100);
 tempC = readTemp(tmpPin);
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
    data[0] = 48 ;
    data[1] = 68;
    data[2]= 88;
   }
 }
}
 
// callback for sending data
void sendData(){
 Wire.write(data,3);
 }

float readTemp(byte tempPin)
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
  Serial.print("   celcius = ");
  Serial.println(celcius,1);
  return celcius;
}

 

