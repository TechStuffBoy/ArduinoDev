#include<EEPROM.h>
byte AddressEEPROM = 0;

void setup(){
  Serial.begin(9600);
  Serial.print(F(" Arduino ID Is : "));
  Serial.println(EEPROM.read(AddressEEPROM));
}

void loop(){
   
}
