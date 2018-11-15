const unsigned long sampleTime = 100000UL;                           // sample over 100ms, it is an exact number of cycles for both 50Hz and 60Hz mains
const unsigned long numSamples = 250UL;                               // choose the number of samples to divide sampleTime exactly, but low enough for the ADC to keep up
const unsigned long sampleInterval = sampleTime/numSamples;  // the sampling interval, must be longer than then ADC conversion time
const int adc_zero = 512;                                                     // relative digital zero of the arudino input from ACS712 (could make this a variable and auto-adjust it)
const int currentPin = 0;
#define light 12
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    unsigned long currentAcc = 0;
    unsigned int count = 0;
    int sensorValue = 0;
    for (count = 0; count < numSamples; count++) {
        int adc_raw = analogRead(currentPin) - adc_zero;
        currentAcc += (unsigned long)(adc_raw * adc_raw);
        delayMicroseconds(sampleInterval);
    }
   /*
   The 50 for the conversion comes from solving for X, 5 V / X = .100 V / 1 A
   0.185 for 5A model, 0.100 For 20A Model, 0.066 for 30A Model 
   */
    float rms = sqrt((float)currentAcc/(float)numSamples) * (92.593 / 1024.0); // For WCS2720
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (75.7575 / 1024.0); // For 30A Model
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (50 / 1024.0); // For 20A Model
    //float rms = sqrt((float)currentAcc/(float)numSamples) * (27 / 1024.0); // For 5A Model
    float rmsAcc = rms - 0.07; //subtract the stray value, It Will change for sensor to sensors.
    
    float power = 230 * (rmsAcc); // P= VI
    if((rmsAcc > 0)) {
        Serial.print("Current Flowing: ");
        Serial.print(rmsAcc);
        Serial.print("A     Power Consumed:");
        Serial.print(power);
        Serial.println("W");
        if( rmsAcc >= 0.40)
            turnOnLight();
    } else {
        turnOffLight();
        Serial.println("No Load is Switched ON");
    }
    currentAcc = 0;
    delay(1000);
}

void turnOnLight()
{
    Serial.println("Led is ON");
    digitalWrite(light,HIGH);
}
void turnOffLight()
{
    Serial.println("Led is OFF");
    digitalWrite(light,LOW);
}

