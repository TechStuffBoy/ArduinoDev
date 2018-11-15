#include<EEPROM.h>
int addr =0;


void setup(){
  Serial.begin(9600);
  EEPROM.write(addr,001);
}

void loop(){
 // byte value=EEPROM.read(addr);
  Serial.print(F("Value in EEPROM in addr_0 :"));
  Serial.println( EEPROM.read(addr));
}

