const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;
unsigned int count = 0;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 unsigned long prevMicros = micros() - sampleInterval ;
 Serial.print("PrevMicros before while:");
 Serial.println(prevMicros);
 while (count < numSamples)
 {
   if (micros() - prevMicros >= sampleInterval)
   {
     ++count;
     prevMicros += sampleInterval;
      Serial.print("In While after while ,PrevMicros:");
      Serial.println(prevMicros);
   }
 }
 delay(5000);
}

