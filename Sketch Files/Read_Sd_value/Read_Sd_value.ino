#include <SPI.h>
#include <SD.h>

File myFile;

void setup()
{
  Serial.begin(9600);


  Serial.print("Initializing SD card...");

  pinMode(10, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  myFile = SD.open("datalog.txt");
  if (myFile) {
    Serial.println("datalog.txt:");

    while (myFile.available()) {
      Serial.write(myFile.read());  //THIS LINE RIGHT HERE
    }
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop()
{
}
