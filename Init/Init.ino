//Imports necessary libraries for sensors
#include <SharpDistSensor.h>
#include <Encoder.h>

SharpDistSensor LeftSide(pin, 1); //pin is a placeholder variable
SharpDistSensor RightSide(pin, 1); //pin is a placeholder variable
SharpDistSensor Front(pin, 1); //pin is a placeholder variable
Encoder LeftWheel(const pin = 2, pin); //second pin is a placeholder variable
Encoder RightWheel(const pin = 3, pin); //second pin is a placeholder variable
pinMode(rightMotorPin, OUTPUT); //Right Motor
pinMode(leftMotorPin, OUTPUT); //Left Motor

//Initiates Session
void setup() {
  LeftSide.setModel(SharpDistSensor::GP2Y0A51SK0F);
  RightSide.setModel(SharpDistSensor::GP2Y0A51SK0F);
  Front.setModel(SharpDistSensor::GP2Y0A51SK0F);
  Serial.Begin(9600);
}

void loop() {
  
  option = runOption();
  if(option == 0) {
    moveForward();
  } else if(option == 1) {
    turnRight();
    turnRight();
  } else if(option == 2) {
    if(random(2) == 0) {
      turnLeft();
      moveForwardOneCarLength();
    }
  } else if(option == 3) {
    if(random(2) == 0) {
      turnRight();
      moveForwardOneCarLength();
    }
  }
}


int runOption() {
  if(LeftSide.getDist() < 50 && RightSide.getDist() < 50 && Front.getDist() < 50) {
    return 1;
  } else if(LeftSide.getDist() > 50) {
    return 2;
  } else if(RightSide.getDist() > 50) {
    return 3;
  }
  return 0;
}

//Starts both motors in the forward direction
void moveForward() {
  analogWrite(rightMotorPin, 128);
  analogWrite(leftMotorPin, 128);
}

void moveForwardOneCarLength() {
  encoderReset();
  while (LeftWheel.read() < 90 && RightWheel.read() < 90) {
    moveForward();
  }
}

//Stops all motor movement
void stopMoving() {
  analogWrite(rightMotorPin, 0);
  analogWrite(leftMotorPin, 0);
}

//Makes robot turn left at a 90 degree angle
//Current values of 90 and -90 are experimental
void turnRight() {
  encoderReset();
  while (LeftWheel.read() < 90 && RightWheel.read() > -90) {
    analogWrite(rightMotorPin, -128);
    analogWrite(leftMotorPin, 128);
  }
  stopMoving();
}

//Makes robot turn left at a 90 degree angle
//Current values of -90 and 90 are experimental
void turnLeft() {
  encoderReset();
  while (LeftWheel.read() < -90 && RightWheel.read() > 90) {
    analogWrite(rightMotorPin, 128);
    analogWrite(leftMotorPin, -128);
  }
  stopMoving();
}

//Resets encoder value to 0 for comparison
void encoderReset() {
  LeftWheel.write(0);
  RightWheel.write(0);
}

