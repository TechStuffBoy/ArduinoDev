#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
 
#include <AFMotor.h>
AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor2(2, MOTOR12_64KHZ);
 
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
int valMotor1;
int valMotor2;
void setup()
{
Wire.begin();
Serial.begin(9600);
Serial.println("Initialize MPU");
mpu.initialize();
Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
 
motor1.setSpeed(100); // set the speed to 200/255
motor2.setSpeed(100);
 
}
void loop()
{
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
ax = map(ax, -17000, 17000, -125, 125);
 
valMotor1 = 125+ax;
valMotor2 = 125-ax;
 
Serial.println(valMotor1, DEC);
Serial.println(valMotor2, DEC);
 
if(valMotor1 > 0 && valMotor1 < 255){
motor1.setSpeed(valMotor1);
}
if(valMotor2 > 0 && valMotor2 < 255){
motor2.setSpeed(valMotor2);
}
 
motor1.run(FORWARD); // turn it on going forward
motor2.run(BACKWARD);
 
delay(200);
 
}
