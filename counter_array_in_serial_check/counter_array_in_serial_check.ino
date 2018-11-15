unsigned int counter[] = {0,0,0,0};

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if(Serial.available() ){
        char c = Serial.read();
        Serial.println(c);
        if( c == 'a' ) {
            counter[0] = counter[0]+1;
        }
        if( c == 'b' ) {
            counter[1] = counter[0]+1;
        }
        if( c == 'c' ) {
            counter[2] = counter[0]+1;
        }
        if( c == 'd' ) {
            counter[3] = counter[0]+1;
        }
        if( c == 'E' ) {
            Serial.print( "Counter val of a:");
            Serial.println( counter[0] );
            Serial.print( "Counter val of b:");
            Serial.println( counter[1] );
            Serial.print( "Counter val of c:");
            Serial.println( counter[2] );
            Serial.print( "Counter val of d:");
            Serial.println( counter[3] );
            memset( counter ,0, sizeof(counter));
            Serial.println("Cleared Counter Array");
            return;
        }
    }
}

