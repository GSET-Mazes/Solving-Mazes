//Imports necessary libraries for sensors
#include <Encoder.h>
#include <NewPing.h>
#include <Map.h>
#include <NewPing.h>

//pins
const byte rightIRSensor = 0;
const byte leftIRSensor = 0;
const byte frontUSSensor = 0;
const byte leftEncoder1 = 0;
const byte leftEncoder2 = 0;
const byte rightEncoder1 = 0;
const byte rightEncoder2 = 0;
const byte leftWheel = 0;
const byte rightWheel = 0;
const byte ultraSonicTrigger = 0;
const byte ultraSonicEcho = 0;

Encoder leftWheelEncoder(leftEncoder1, leftEncoder2); //second pin is a placeholder variable
Encoder rightWheelEncoder(rightEncoder1, rightEncoder2); //second pin is a placeholder variable
NewPing sonar(ultraSonicTrigger, ultraSonicEcho, 3000);


//Initiates Session
void setup() {
  pinMode(leftWheel, OUTPUT); //Left Motor
  pinMode(rightWheel, OUTPUT); //Right Motor 
  Serial.Begin(9600);
}

void loop() {
  option = runOption();
  if (option == 0) {
    moveForward();
  } else if (option == 1) {
    moveForwardOneCarLength();
    switch (random(3)) {
      case 0:
        moveForwardOneCarLength();
        break;
      case 1:
        turnLeft();
        moveForwardOneCarLength();
        break;
      case 2:
        turnRight();
        moveForwardOneCarLength();
        break;
    }
  } else if (option == 2) {
    moveForwardOneCarLength();
    switch (random(2)) {
      case 0:
        turnLeft();
        moveForwardOneCarLength();
        break;
      case 1:
        turnRight();
        moveForwardOneCarLength();
        break;
    }
  } else if (option == 3) {
    moveForwardOneCarLength();
    if (random(2) == 0) {
      turnRight();
      moveForwardOneCarLength();
    } else {
      moveForwardOneCarLength();
    }
  } else if (option == 4) {
    moveForwardOneCarLength();
    if (random(2) == 0) {
      turnLeft();
      moveForwardOneCarLength();
    } else {
      moveForwardOneCarLength();
    }
  } else if (option == 5) {
    moveForwardOneCarLength();
    turnRight();
    moveForwardOneCarLength();
  } else if (option == 6) {
    moveForwardOneCarLength();
    turnLeft();
    moveForwardOneCarLength();
  } else if (option == 7) {
    turnLeft();
    turnLeft();
  }
}

/**
   Check what turning options are available at specific point
   //front has to be right at beginning of intersection
*/
int runOption() {
  double rightIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3;
  double leftIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3;
  double frontUSSensorValue = sonar.ping_cm();
  if (leftIRSensorValue > 7 && rightIRSensorValue > 7 && frontUSSensorValue > 7) { //@ a 4-way
    return 1;
  } else if (leftIRSensorValue > 7 && rightIRSensorValue > 7) { //@ a 3-way
    return 2;
  } else if (rightIRSensorValue > 7 && frontUSSensorValue > 7) { //@ another 3-way: front, right, back
    return 3;
  } else if (leftIRSensorValue > 7 && frontUSSensorValue > 7) { //@ another 3-way: front, left, back
    return 4;
  } else if (rightIRSensorValue > 7) { //@ a 2-way: right, back
    return 5;
  } else if (leftIRSensorValue > 7) { //@ a 2-way: left, back
    return 6;
  } else if (frontUSSensorValue < 7) { //@ a dead-end
    return 7;
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

