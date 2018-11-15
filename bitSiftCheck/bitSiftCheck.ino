byte data = 0x00;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    /* Setting Particular Bit */
    data = 0x00;
    Serial.println(data,BIN);
    //put your main code here, to run repeatedly:
    Serial.print("data | (1<<0) :");
    data = data | (1<<0);
    Serial.println( data , BIN);
    data = 0x00;
    Serial.print("data | (1<<1) :");
    data = data | (1<<1);
    Serial.println( data , BIN);
    data = 0x00;
    Serial.print("data | (1<<2) :");
    data = data | (1<<2);
    Serial.println( data , BIN);
    /* Setting Particular Bit */

    /*Clearing Particular Bit */
    data = 0xFF;
    Serial.println(data,BIN);
    Serial.print("data & (~(1<<0)):");
    data = data & (~(1<<0));
    Serial.println(data,BIN);
    data = 0XFF;
    Serial.print("data & (~(1<<1)):");
    data = data & (~(1<<1));
    Serial.println(data,BIN);
    data = 0xff;
    Serial.print("data & (~(1<<2)):");
    data = data & (~(1<<2));
    Serial.println(data,BIN);
    /*Clearing Particular Bit */
    Serial.println();
    Serial.println();
    data = 0x00;
    data = data | ( 1 << PC7);
    Serial.println(data,BIN);
    data = 0x00;
    data = data | ( 1 << 6);
    Serial.println();
    Serial.println(data,BIN);
    while(1);
}
