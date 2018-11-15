// Insert two libraries: Servo.h (for your Servo to work) and math.h (for inverse tangent function).

#include<Servo.h>
#include <math.h>

// Name your servo.  I called mine: servo.

Servo servo;

// Since "pi" has not been defined in any of the available libraries, you must define it yourself.

const float pi = 3.14159265359;

// Set up some floats: (1) PersonX and PersonY (position of person in relation to a coordinate system), 
// (2) MotorX and MotorY (position of motor in relation to such coordinate system),
// (3) deltaX and deltaY (difference between the position of person in relationship to the position of the motor,
// and (4) angleRad and angleDeg (angle of motor to person in both radians and degrees).

float PersonX, PersonY;
float MotorX, MotorY;
float deltaX, deltaY;
float angleRad, angleDeg;

// In void setup(), make sure to set up a baudrate for your serial monitor 
// and to mention what pin your Servo is attached to.

void setup() {
  Serial.begin(9600);
  servo.attach(3);
}

void loop() {
  // Give the motor a position:
  MotorX = 2.5; 
  MotorY = 2.5;
  
  // Now give the person a series of positions:
  motorTurn(1,1);
  delay(1000);
  motorTurn(2,1);
  delay(1000);
  motorTurn(3,1);
  delay(1000);
  motorTurn(4,1);
  delay(1000);
  motorTurn(5,1);
  delay(1000);
}

// The motor should now rotate towards the position of the person and eventually follow the person through each position within the series provided.

// Let's declare a function, and let us call that function motorTurn.

void motorTurn(int PersonX,int PersonY) { // PersonX is a person's location on the x-axis, while PersonY is a person's location on the y-axis.

  // As mentioned before, deltaX and deltaY is the difference between the position of a person in relation to the position of the motor.

  deltaX = PersonX - MotorX;
  deltaY = PersonY - MotorY;
  
  if (deltaX > 0 ) {
    angleRad = atan(deltaY/deltaX);
  }
  else {
    angleRad = pi + atan(deltaY/deltaX);
  }
  
  angleDeg = angleRad * (180/pi); // Let us convert the angle from radians (angleRad) to degrees (angleDeg).

  if (angleDeg >180) {
    angleDeg = -(360-angleDeg); // Now, let us keep the angles between -180 to 180 degrees, for the sake of clarity.
  }
  
  Serial.print("Coordinate: ");
  Serial.print(PersonX);
  Serial.print(",");
  Serial.println(PersonY);
  Serial.print("Angle: ");
  Serial.println(angleDeg);
  Serial.println(" degrees");
  
  servo.write(angleDeg);  // This should make the Servo rotate.
  delay(1000);
}
