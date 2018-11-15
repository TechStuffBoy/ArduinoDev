String exString = "Hello World!";    // example string
int a =0;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // send an intro:
  Serial.println("\n\nString remove() method example");
  Serial.println();
}

void loop() {
    a = exString.length();
    Serial.print("exString length :");
    Serial.println(a);
    Serial.println("The full string:");
    Serial.println(exString);
    
    exString.remove(7);
    a = exString.length();// Remove from from index=7 through the end of the string
    Serial.println("String after removing from the seventh index through the end");
    Serial.println(exString);  // Should be just "Hello W"
    Serial.print("exString length :");
    Serial.println(a);
    
    exString = "Hello World!";
    exString.remove(2, 6); // Remove six characters starting at index=2
    a = exString.length();
    Serial.println("String after removing six characters starting at the third position");
    Serial.println(exString); // Should be just "Herld!"
    Serial.print("exString length :");
    Serial.println(a);
    Serial.println();
    Serial.println();
    
    exString= "";
    a = exString.length();
    Serial.print("exString length :");
    Serial.println(a);
    Serial.println();
 
  while (1)
    ; // no need to do it again
    Serial.println("i am in endddddddddd");
}
