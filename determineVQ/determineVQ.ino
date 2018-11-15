
int adc_zero = 0;
void setup() {
    Serial.begin(9600);
    Serial.println("calculating Quiscent voltage...\nPlease Wait");
  // put your setup code here, to run once:
    adc_zero = determineVQ(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Val is :");
  Serial.println(adc_zero);
  float voltage = adc_zero * (5.0 / 1023.0);
  Serial.print("Preset voltage is :");
  Serial.println(voltage);
  delay(2000);
}

int determineVQ(int PIN) {
  Serial.print("estimating avg. quiscent voltage:");
  long VQ = 0;
  //read 5000 samples to stabilise value
  for (int i = 0; i < 5000; i++) {
    VQ += analogRead(PIN);
    delay(1);//depends on sampling (on filter capacitor), can be 1/80000 (80kHz) max.
  }
  VQ /= 5000;
  Serial.print(map(VQ, 0, 1023, 0, 5000)); Serial.println(" mV");
  return int(VQ);
}

