#define LED_PIN 13

void setup()
{
	Serial.begin(9600);
	pinMode(LED_PIN,OUTPUT);
}

void loop()
{
	Serial.println("Led on");
	digitalWrite(LED_PIN,HIGH);
	delay(500);
	Serial.println("Led off");
	digitalWrite(LED_PIN,LOW);
	delay(500);
	
}