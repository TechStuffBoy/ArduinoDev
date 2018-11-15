#define LED_PIN 13

void setup()
{
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
}

void loop()
{
	Serial.println(F("Hi Man!!"));
	delay(100);
	Serial.println(F("How are you!!"));
	delay(100);
	Serial.println(F("How about atmel studio"));
	delay(100);
	
}