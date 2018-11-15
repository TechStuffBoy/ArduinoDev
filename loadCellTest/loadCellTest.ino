#include "HX711.h"

// HX711.DOUT    - pin #A1
// HX711.PD_SCK - pin #A0
//HX711 scale(DOUT, CLK);
HX711 scale(A1, A0);        // parameter "gain" is ommited; the default value 128 is used by the library

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Demo");

 Serial.println("Before setting up the scale:");
 Serial.print("read: \t\t");
 Serial.println(scale.read());            // print a raw reading from the ADC
}

void loop()
{
   readCell_1();
}

void readCell()
{
  long preset = 930000;
  float grams = 100.0;
  int phoneWeight = 155;
  long analogPhoneWeight = 1155000;
  float value = 0;
  float val = 0;
  val = (analogPhoneWeight - preset)/155;
  value = ((scale.read()-preset)/val);
  
  if( value > 0 )
    Serial.println(value);
  else{
    value = 0 ;
    //Serial.println(value);
  }
}

void readCell_1()
{
  long preset = 1151000;
  float phoneWeight = 155;
  long analogPhoneWeight = 1384000;
  float value = 0;
  float val = 0;
  val = (analogPhoneWeight - preset)/phoneWeight; //Add preset here also
  value = ((scale.read()-preset)/val);
  //value = value - 168.0; //PresetvalForLoad
  if(value > 0 )
    Serial.println(value);
  else{
    value = 0;
    Serial.println(value);
  }
}

/*
 * void readCell_1()
{
  long preset = 920000;
  float grams = 100.0;
  int phoneWeight = 155;
  long analogPhoneWeight = 1132568;
  float value = 0;
  float val = 0;
  val = (analogPhoneWeight - preset)/155;
  value = ((scale.read()-preset)/val);
  
  //if( value > 0 )
    Serial.println(value);
//  else{
//    value = 0 ;
//    //Serial.println(value);
//  }
}

 */
void readContainer()
{
  long preset = 930000;
  float grams = 100.0;
  int containerWeight = 213;
  long analogContainerWeight = 1239000 ;
  float value = 0;
  float val = 0;
  val = (analogContainerWeight - preset)/213;
  value = ((scale.read()-preset)/val);
  Serial.println(value);
}

