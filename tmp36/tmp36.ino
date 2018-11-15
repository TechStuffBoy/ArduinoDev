//#define aref_voltage 3.25

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //analogReference(EXTERNAL);

}

void loop() {
  // put your main code here, to run repeatedly:
  float rawAdcVal = 0;
  float voltage = 0;
  float volInMilli = 0;
  float celcius = 0;
  float farenheit = 0;
  for(int i=1;i<200;i++){
    rawAdcVal += analogRead(0);
    delay(5);
  }
  rawAdcVal /= 200.0;
  voltage = (5 * rawAdcVal) / 1024;
  //voltage = (aref_voltage * rawAdcVal) / 1024; //for 3.3v
  volInMilli = voltage * 1000;
  celcius = (volInMilli - 500) / 10; // 500 is offset for tmp36,10 is 10mv precision.
  farenheit = (celcius * 9.0 / 5.0) + 32.0;
  
  /* Print Statements */
  
  Serial.print("Raw Voltage = ");
  Serial.print(rawAdcVal);
  Serial.print("   Voltage = ");
  Serial.print(voltage);
  Serial.print("   volInMilli = ");
  Serial.print(volInMilli,1);
  Serial.print("   celcius = ");
  Serial.print(celcius,1);
  Serial.print("   farenheit = ");
  Serial.println(farenheit,1);
  delay(1000);
}
