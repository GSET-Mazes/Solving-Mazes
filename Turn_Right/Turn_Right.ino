//Imports necessary libraries for sensors
#include <Encoder.h>

const byte rightIRSensor = 0;
const byte leftIRSensor = 0;
const byte frontUSSensor = 0;
const byte leftWheel = 0;
const byte rightWheel = 0;

Encoder leftWheelEncoder(leftWheel, pin); //second pin is a placeholder variable
Encoder rightWheelEncoder(rightWheel, pin); //second pin is a placeholder variable
pinMode(leftWheel, OUTPUT); //Right Motor
pinMode(rightWheel, OUTPUT); //Left Motor

//Initiates Session
void setup() {
  Serial.begin(9600);
}

void loop() {
  option = runOption();
  if(option == 0) {
    moveForward();
  } else if(option == 1) {
    moveForwardOneCarLength();
    turnRight();
    moveForwardOneCarLength();
  } else if(option == 2) {
    moveForwardOneCarLength();
    turnLeft();
    moveForwardOneCarLength();
  } else if(option == 3) {
    turnLeft();
    turnLeft();
  }
}

int runOption() {
  double rightIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3;
  double leftIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3;
  double frontUSSensorValue;
  if(rightIRSensorValue > 7) { //@ a 4-way
    return 1;
  } else if(leftIRSensorValue > 7 && frontUSSensorValue < 7) { //@ a 3-way
    return 2;
  } else if(frontUSSensorValue < 7) { //@ a right path
    return 3;
  }
  return 0;
}


//Starts both motors in the forward direction
void moveForward() {
  analogWrite(rightWheel, 128);
  analogWrite(leftWheel, 128);
}
void moveForwardOneCarLength() {
  encoderReset();
  while (LeftWheel.read() < 90 && RightWheel.read() < 90) {
    moveForward();
  }
  stopMoving();
}

void moveBackward() {
  analogWrite(leftWheel, -128);
  analogWrite(rightWheel, -128);
}
void moveBackwardOneCarLength() {
  encoderReset();
  while (LeftWheel.read() > -90 && RightWheel.read() > -90) {
    moveBackward();
  }
  stopMoving();
}

//Stops all motor movement
void stopMoving() {
  analogWrite(rightWheel, 0);
  analogWrite(leftWheel, 0);
}

//Makes robot turn left at a 90 degree angle
//Current values of 90 and -90 are experimental
void turnRight() {
  encoderReset();
  while (leftWheelEncoder.read() < 90 && rightWheelEncoder.read() > -90) {
    analogWrite(rightWheel, -128);
    analogWrite(leftWheel, 128);
  }
  stopMoving();
}

//Makes robot turn left at a 90 degree angle
//Current values of -90 and 90 are experimental
void turnLeft() {
  encoderReset();
  while (leftWheelEncoder.read() < -90 && rightWheelEncoder.read() > 90) {
    analogWrite(rightMotorPin, 128);
    analogWrite(leftMotorPin, -128);
  }
  stopMoving();
}

//Resets encoder value to 0 for comparison
void encoderReset() {
  leftWheelEncoder.write(0);
  rightWheelEncoder.write(0);
}

