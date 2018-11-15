#include "BButil.h"
#define MOTOR_ENABLE_PIN 3
#define MOTOR_DIRA_PIN 4
#define MOTOR_DIRB_PIN 5
#define LED_PIN 13

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include <Arduino.h>
MPU6050 mpu;


// MPU control/status vars
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64];          // FIFO storage buffer
 
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
 
VectorFloat gravity;    // [x, y, z]            gravity vector
float yawPitchRoll[3];  // [z, y, x]   yaw/pitch/roll container
int16_t gyros[3];      // [x, y, z] gyros container
 
bool mpu_setup() {
    Wire.begin(); // join I2C bus (I2Cdev library doesn't do this automatically)
 
    Serial.print(F("Init I2C "));
    mpu.initialize();
 
    Serial.print(F("ID")); Serial.println(mpu.getDeviceID());
 
    // verify connection
    Serial.println(F("Test conns"));
    Serial.println(mpu.testConnection() ? F("Conn success") : F("Conn failed"));
 
    // load and configure the DMP
    Serial.println(F("Init DMP"));
    uint8_t devStatus = mpu.dmpInitialize();
 
    // 0 = success, !0 = error
    if (devStatus == 0) {
        Serial.println(F("Enable DMP"));
        mpu.setDMPEnabled(true);
 
        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
 
        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("Set INTrrpts"));
 
        return true;
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP ERR ")); Serial.println(devStatus);
 
        return false;
    }
}
 
int8_t mpu_getData(AngleGyroData* data){
    uint8_t mpuIntStatus = mpu.getIntStatus();struct AngleGyroData {
    float angle;
    int16_t gyro;
};
 
    // get current FIFO count (all bytes currently in the FIFO)
    uint16_t fifoCount = mpu.getFIFOCount();
 
    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount >= 1024) {
        mpu.resetFIFO(); // reset so we can continue cleanly
        return -1; //FIFO overflow
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        if (fifoCount >= packetSize) {
          // read 1st packet from FIFO, don't bother with the rest
          mpu.getFIFOBytes(fifoBuffer, packetSize);
          mpu.resetFIFO(); // reset so we can continue cleanly
        }
 
        // in case there were plenty of packets in the FIFO, only transform the last one, to avoid wasting CPU for nothing
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(yawPitchRoll, &q, &gravity);
        mpu.dmpGetGyro(gyros, fifoBuffer);
 
        // all we care is the Y axis data
        data->angle = yawPitchRoll[1];
        data->gyro =  gyros[1];
 
        return 0; // all good
    }else {
      return -2; //Wrong Status
    }
}
 
// The temperature sensor is -40 to +85 degrees Celsius.
// It is a signed integer.
// According to the datasheet: 340 per degrees Celsius, -512 at 35 degrees.
// At 0 degrees: -512 - (340 * 35) = -12412
int8_t mpu_getTemp(){
  return (mpu.getTemperature() + 12412.0) / 340.0;
}

// angle is in radians
// gyro is in radians per sec, doesn't normally go beyond 1000
// the 2 have opposite signs
AngleGyroData _angleData = {0, 0};
 
float _initAngle;
 
void setup(){
    Serial.begin(115200);
 
    mpu_setup();
 
    pinMode(MOTOR_ENABLE_PIN, OUTPUT);
    pinMode(MOTOR_DIRA_PIN, OUTPUT);
    pinMode(MOTOR_DIRB_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
 
    // configure the initial angle, which is assumed to be the "stable" position
    delay(1000);
    while(mpu_getData(&_angleData));//wait to get valid data
    _initAngle = _angleData.angle;
}
 
long currentTime, lastTime = 0;
float integralErr = 0, INTEGRAL_ERR_MAX = 10, actualAngle, ANGLE_OFFSET = 0.035;
 
void loop(){
    currentTime = millis();
 
    if(currentTime - lastTime > 1){
        // get the MPU data if available
        int8_t res = mpu_getData(&_angleData);
 
        // if res != 0 the data is not yet ready or there were errors, so ignore and keep trying
        if(res == 0){
            actualAngle = _angleData.angle - ANGLE_OFFSET;
 
            // 1. update the speed, apply PID algo (for the D, we don't need to do it manually as the sensor already gives us the gyro value which is the derivative of the angle)
            int16_t speed = 1200 * actualAngle + 90 * integralErr - 1.5 * _angleData.gyro;
            //if(speed > -50 && speed < 50) speed = 0;
            speed = constrain(speed, -255, 255);
            analogWrite(MOTOR_ENABLE_PIN, map(abs(speed), 0, 255, 50, 255));
            //analogWrite(MOTOR_ENABLE_PIN, abs(speed));
            integralErr = constrain(integralErr + actualAngle, -INTEGRAL_ERR_MAX, INTEGRAL_ERR_MAX);
 
            // 2. figure out which DIRECTION to go
            if(speed > 0){
                digitalWrite(MOTOR_DIRA_PIN, HIGH);
                digitalWrite(MOTOR_DIRB_PIN, LOW);
 
                digitalWrite(LED_PIN, LOW);
            }else{
                digitalWrite(MOTOR_DIRA_PIN, LOW);
                digitalWrite(MOTOR_DIRB_PIN, HIGH);
 
                digitalWrite(LED_PIN, HIGH);
            }
    //Serial.print(_angleData.angle); Serial.print(" / "); Serial.print(_angleData.gyro); Serial.print(" @ "); Serial.println(speed);
 
            // 3. keep track of the timings so that we do the update at regular intervals
            lastTime = currentTime;
        }
    }
}
 
 

 

 
//
// 
//#ifndef BBUTIL_h
//#define BBUTIL_h
// 
//#include <Arduino.h>
// 
//struct AngleGyroData {
//    float angle;
//    int16_t gyro;
//};
// 
//#endif
