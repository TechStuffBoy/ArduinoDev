
#define GREEN_LED 13

void setup()
{
	Serial.begin(9600);
	pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
	Serial.println(F("I am from new mega"));
	digitalWrite(GREEN_LED, HIGH);
	delay(1000);
	digitalWrite(GREEN_LED, LOW);
	delay(1000);
	Serial.println(F("Testing End"));
}