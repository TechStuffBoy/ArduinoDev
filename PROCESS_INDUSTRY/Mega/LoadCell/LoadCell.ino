#include "HX711.h"

// HX711.DOUT    - pin #A1
// HX711.PD_SCK - pin #A0
//HX711 scale(DOUT, CLK);
HX711 scale(A1, A0);        // parameter "gain" is ommited; the default value 128 is used by the library
void setup() 
{
    Serial.begin(9600);
    Serial.println("HX711 Demo");
    Serial.println("Before setting up the scale:");
    Serial.print("read: \t\t");
    Serial.println(scale.read());            // print a raw reading from the ADC
}

void loop()
{
   readCell();
}


void readCell()
{
  long preset = 1151000;
  float phoneWeight = 155;
  long analogPhoneWeight = 1384000; //Raw value For Phone
  float value = 0;
  float val = 0;
  val = (analogPhoneWeight - preset)/phoneWeight; //Add preset here also
  value = ((scale.read()-preset)/val);
  if( value > 0 )
    Serial.println(int(value));
  else{
    value = 0;
    Serial.println(value);
  }
}

