char intBuffer[12];
String intData = "";
int delimiter = (int) '\n';

void setup() {
    Serial.begin(9600);
}

void loop() {
    while (Serial.available()) {
        int ch = Serial.read();
        if (ch == -1) {
            // Handle error
        }
        else if (ch == delimiter) {
            break;
        }
        else {
            intData += (char) ch;
        }
    }

    // Copy read data into a char array for use by atoi
    // Include room for the null terminator
    int intLength = intData.length() + 1;
    intData.toCharArray(intBuffer, intLength);

   

    // Convert ASCII-encoded integer to an int
    int i = atoi(intBuffer);
    
    if( i == 22){
        Serial.println("22 is given");
    }
    // Reinitialize intData for use next time around the loop
    intData = "";
    memset(intBuffer,0,sizeof(intBuffer));
}
