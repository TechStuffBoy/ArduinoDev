float voltage = 0.0;
float current = 0.0;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = 0;
  for (byte x = 0; x < 50; x++) {
    sensorValue += analogRead(A0);
    //delayMicroseconds(100);
  }
  sensorValue = sensorValue / 50;
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = sensorValue * (5.0 / 1023.0);
  current = (voltage - 2.50 )/0.185;
  // print out the value you read:
  Serial.print("voltage :");
  Serial.print(voltage);
  Serial.print("  current :");
  Serial.println(current);
  delay(1000);
}


