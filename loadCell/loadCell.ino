#include <Hx711.h>
Hx711 scale(A1, A0);
//HX711 scale(DOUT, CLK);
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(scale.getGram(), 1);
  Serial.println(" g");
  delay(1);
}
