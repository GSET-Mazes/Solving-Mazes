//Imports necessary libraries for sensors
#include <Encoder.h>
#include <NewPing.h>
#include <Map.h>
#include <NewPing.h>

//pins
const byte rightIRSensor = 11; //$
const byte leftIRSensor = 10; //$
const byte leftEncoder1 = 0; //$
const byte leftEncoder2 = 0; //$
const byte rightEncoder1 = 12; //$
const byte rightEncoder2 = 2; //$
const byte leftWheel = 0; //$
const byte rightWheel = 0; //$
const byte ultraSonic = 6; //$

Encoder leftWheelEncoder(leftEncoder1, leftEncoder2);
Encoder rightWheelEncoder(rightEncoder1, rightEncoder2);
NewPing sonar(ultraSonic, ultraSonic, 3000);
Map mapStart;
Node *curr;

int steps = 0;
bool onDeadEnd = false;
int lastTurn = 0;
double distanceFromWalls = 7; //$

//Initiates Session
void setup() {
  pinMode(leftWheel, OUTPUT); //Left Motor
  pinMode(rightWheel, OUTPUT); //Right Motor
  Serial.begin(9600);
  if(runOption() == 0) { //Ishan is awesome
    curr = mapStart.addPath(mapStart.getHead(), 0);
    moveForwardOneCarLength();
  }
}

void loop() {
  int option = runOption();
  if(onDeadEnd) {
    double rightIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3;
    double leftIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3;
    double frontUSSensorValue = sonar.ping_cm();
    moveBackward();
    if(leftIRSensorValue > distanceFromWalls || rightIRSensorValue > distanceFromWalls) {
      moveBackwardOneCarLength();
      int tempTurn = mapStart.getTurn(curr);
      curr = mapStart.getPreviousNode(curr);
      switch(tempTurn) {
        case 1:
          turnRight();
          if(!!mapStart.getPath(curr, 0)) {
            moveForwardOneCarLength();
            onDeadEnd = false;
          } else {
            mapStart.makeDeadEnd(curr);
            moveBackwardOneCarLength();
          }
          break;
        case 2:
          turnLeft();
          if(!!mapStart.getPath(curr, 1)) {
            turnLeft();
            moveForwardOneCarLength();
            onDeadEnd = false;
          } else if(!!mapStart.getPath(curr, 0)) {
            moveForwardOneCarLength();
            onDeadEnd = false;
          } else {
            mapStart.makeDeadEnd(curr);
            moveBackwardOneCarLength();
          }
          break;
      }
      
    }
  } else {
      if (option == 0) {
        moveForward();
      } else if (option == 1) {
        moveForwardOneCarLength();
        fourWay();
        turnRight();
        moveForwardOneCarLength();
      } else if (option == 2) {
        moveForwardOneCarLength();
        sidesThreeWay();
        turnRight();
        moveForwardOneCarLength();
      } else if (option == 3) {
        moveForwardOneCarLength();
        rightThreeWay();
        turnRight();
        moveForwardOneCarLength();
      } else if (option == 4) {
        moveForwardOneCarLength();
        leftThreeWay();
        turnLeft();
        moveForwardOneCarLength();
      } else if (option == 5) {
        moveForwardOneCarLength();
        rightTwoWay();
        turnRight();
        moveForwardOneCarLength();
      } else if (option == 6) {
        moveForwardOneCarLength();
        leftTwoWay();
        turnLeft();
        moveForwardOneCarLength();
      } else if (option == 7) {
        stopMoving();
        deadEnd();
      }
  }

}

/**
   Check what turning options are available at specific point
   //front has to be right at beginning of intersection
*/
int runOption() {
  double rightIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3; //using exponential regression stuff that Karan did
  double leftIRSensorValue = 39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3; //using exponential regression stuff that Karan did
  double frontUSSensorValue = sonar.ping_cm();
  if (leftIRSensorValue > distanceFromWalls && rightIRSensorValue > distanceFromWalls && frontUSSensorValue > distanceFromWalls) { //@ a 4-way
    return 1;
  } else if (leftIRSensorValue > distanceFromWalls && rightIRSensorValue > distanceFromWalls) { //@ a 3-way
    return 2;
  } else if (rightIRSensorValue > distanceFromWalls && frontUSSensorValue > distanceFromWalls) { //@ another 3-way: front, right, back
    return 3;
  } else if (leftIRSensorValue > distanceFromWalls && frontUSSensorValue > distanceFromWalls) { //@ another 3-way: front, left, back
    return 4;
  } else if (rightIRSensorValue > distanceFromWalls) { //@ a 2-way: right, back
    return 5;
  } else if (leftIRSensorValue > distanceFromWalls) { //@ a 2-way: left, back
    return 6;
  } else if (frontUSSensorValue < distanceFromWalls) { //@ a dead-end
    return 7;
  }
  return 0;
}

void fourWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  mapStart.addPath(curr, 0);
  mapStart.addPath(curr, 1);
  curr = mapStart.addPath(curr, 2);
  steps++;
}
void sidesThreeWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  mapStart.addPath(curr, 1);
  curr = mapStart.addPath(curr, 2);
  steps++;
}
void rightThreeWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  mapStart.addPath(curr, 0);
  curr = mapStart.addPath(curr, 2);
  steps++;
}
void leftThreeWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  mapStart.addPath(curr, 0);
  curr = mapStart.addPath(curr, 1);
  steps++;
}
void rightTwoWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  curr = mapStart.addPath(curr, 2);
}
void leftTwoWay() {
  mapStart.setLength(curr, getBlocksTraveled());
  curr = mapStart.addPath(curr, 1);
}
void deadEnd() {
  mapStart.setLength(curr, getBlocksTraveled());
  mapStart.makeDeadEnd(curr);
  onDeadEnd = true;
}




int getBlocksTraveled() {
  int blockEncoderValue = 1; //CHANGE THIS $
  int blocksTraveled = leftWheelEncoder.read() / blockEncoderValue + 1;
  resetEncoders();
  return blocksTraveled;
}





//Starts both motors in the forward direction
void moveForward() {
  analogWrite(rightWheel, 128);
  analogWrite(leftWheel, 128);
}
void moveForwardOneCarLength() {
  resetEncoders();
  while (leftWheelEncoder.read() < 90 && rightWheelEncoder.read() < 90) {
    moveForward();
  }
  stopMoving();
}

void moveBackward() {
  analogWrite(leftWheel, -128);
  analogWrite(rightWheel, -128);
}
void moveBackwardOneCarLength() {
  resetEncoders();
  while (leftWheelEncoder.read() > -90 && rightWheelEncoder.read() > -90) {
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
  resetEncoders();
  while (leftWheelEncoder.read() < 90 && rightWheelEncoder.read() > -90) {
    analogWrite(rightWheel, -128);
    analogWrite(leftWheel, 128);
  }
  stopMoving();
  resetEncoders();
}

//Makes robot turn left at a 90 degree angle
//Current values of -90 and 90 are experimental
void turnLeft() {
  resetEncoders();
  while (leftWheelEncoder.read() < -90 && rightWheelEncoder.read() > 90) {
    analogWrite(rightWheel, 128);
    analogWrite(leftWheel, -128);
  }
  stopMoving();
  resetEncoders();
}

//Resets encoder value to 0 for comparison
void resetEncoders() {
  leftWheelEncoder.write(0);
  rightWheelEncoder.write(0);
}

