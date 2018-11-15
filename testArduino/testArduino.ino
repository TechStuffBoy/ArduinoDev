void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.println("Hello, World!");
	delay(1000);
	Serial.println("This is a message from Arduino Nano");
	delay(1000);
}