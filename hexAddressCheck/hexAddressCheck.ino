unsigned char  num1 = 0x80;
unsigned char  num2 = 15;
unsigned char num3 = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
    setCursor(1,0);
    setCursor(1,1);
    setCursor(0,2);
    setCursor(0,3);
    setCursor(0,4);
    setCursor(0,5);
    setCursor(0,6);
    setCursor(0,7);
    setCursor(0,8);
    setCursor(0,9);
    setCursor(0,10);
    setCursor(0,11);
    setCursor(0,12);
    setCursor(0,13);
    setCursor(0,14);
    setCursor(0,15);
    while(1);
}

void setCursor(unsigned char pos1,unsigned char pos2)
{
    unsigned char address = 0;
    unsigned char LineAddrr = 0;
    if(pos1 == 0){
      LineAddrr = 0x80;  
    } else {
        LineAddrr = 0xC0;
    }
    address = LineAddrr + pos2;
    Serial.println(address,HEX);
    
}
