byte tempPin = 0;
byte vibPin = 1;
#define A        3                     // the pin connected to the wire A of the coil A (or to the H-bridge pin controlling the same wire) 
#define A_bar    4                     // the pin connected to the wire A- of the coil A (or to the H-bridge pin controlling the same wire)
#define B        5                     // the pin connected to the wire B of the coil A (or to the H-bridge pin controlling the same wire)
#define B_bar    6                    // the pin connected to the wire B- of the coil A (or to the H-bridge pin controlling the same wire)
#define x        5000                  // smaller values may make the motor produce more speed and less torque
#define stepsPerRevolution 200 

float vibrationValue = 0;
float tempC = 0;

void setup() 
{
   Serial.begin(9600);
   initializeStepper();
}

void loop() 
{
    driveStepperTwoPhase();
    vibrationValue = checkVibration(vibPin);
    tempC = readTemp(tempPin);
    if( vibrationValue < 4.8 ) {
        Serial.println("Warning!,Vibration Occuring");
    } else if( tempC > 40.0 ){
        Serial.println("Motor is Heating Up,Maintainence required");
    }
}

float checkVibration(byte vibPin)
{
    // Read Piezo ADC value in, and convert it to a voltage
    int piezoADC = analogRead(vibPin);
    float piezoV = piezoADC / 1023.0 * 5.0;
    return piezoV;
}

float readTemp(byte tempPin)
{
     // put your main code here, to run repeatedly:
  float rawAdcVal = 0;
  float voltage = 0;
  float volInMilli = 0;
  float celcius = 0;
  float farenheit = 0;
  for(int i=1;i<200;i++){
    rawAdcVal += analogRead(tempPin);
    delay(5);
  }
  rawAdcVal /= 200.0;
  voltage = (5 * rawAdcVal) / 1024;
  //voltage = (aref_voltage * rawAdcVal) / 1024; //for 3.3v
  volInMilli = voltage * 1000;
  celcius = (volInMilli - 500) / 10; // 500 is offset for tmp36,10 is 10mv precision.
  farenheit = (celcius * 9.0 / 5.0) + 32.0;
  Serial.print("   celcius = ");
  Serial.println(celcius,1);
  return celcius;
}

void initializeStepper()
{
    pinMode(A, OUTPUT);
    pinMode(A_bar, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(B_bar, OUTPUT);
}
void driveStepperTwoPhase()
{
    for (int i = 0; i < (stepsPerRevolution/4) ; i++) {
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
    
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
      }
      /*
      // Counter Direction 
      for (int i = 0; i < (stepsPerRevolution/4); i++) {
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, LOW);
        digitalWrite(B_bar, HIGH);
        delayMicroseconds (x);
    
        digitalWrite(A, LOW);
        digitalWrite(A_bar, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
    
        digitalWrite(A, HIGH);
        digitalWrite(A_bar, LOW);
        digitalWrite(B, HIGH);
        digitalWrite(B_bar, LOW);
        delayMicroseconds (x);
      }
      */
}

