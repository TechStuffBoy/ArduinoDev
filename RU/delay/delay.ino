
void setup()
{
	Serial.begin(9600);
}

void loop()
{
	unsigned long i = 10;
	Serial.println("Delay");
	myDelay(i);
} 

void myDelay(unsigned long sec)
{
	unsigned long delayMSec = sec * 1000;
	unsigned long recordMSec = millis();
	while((millis() - recordMSec) < delayMSec);
}

