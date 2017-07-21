//Imports necessary libraries for sensors
#include <Encoder.h>
#include <NewPing.h>
#include <Map.h>

//pins
const byte button = 1;
const byte rightIRSensor = A1;
const byte leftIRSensor = A4;
const byte ultraSonic = 12;
const byte leftEncoder1 = 2;
const byte leftEncoder2 = 7;
const byte rightEncoder1 = 3;
const byte rightEncoder2 = 9;
const byte leftWheelFwd = 10;
const byte leftWheelBck = 11;
const byte rightWheelFwd = 6;
const byte rightWheelBck = 5;

Encoder leftWheelEncoder(leftEncoder1, leftEncoder2);
Encoder rightWheelEncoder(rightEncoder1, rightEncoder2);
NewPing sonar(ultraSonic, ultraSonic, 3000);
Map mapController;
Node *currentNode;

bool mode = false; //true = solving-mode; false = mapping-mode
bool onDeadEnd = false;

double distanceFromWalls = 7;
int speed = 60;
int quarterTurn = 650;
int oneCarLength = 900;

//Initiates Session
void setup() {
  pinMode(button, INPUT);
  pinMode(leftWheelFwd, OUTPUT); //Left Motor
  pinMode(leftWheelBck, OUTPUT); //Left Motor
  pinMode(rightWheelFwd, OUTPUT); //Right Motor
  pinMode(rightWheelBck, OUTPUT); //Right Motor
  pinMode(leftIRSensor, INPUT);
  pinMode(rightIRSensor, INPUT);
  Serial.begin(9600);
  if (runOption() == 0) { //Ishan is awesome
    currentNode = mapController.addPath(mapController.getHead(), 0);
    moveForwardOneCarLength();
  }
}

void loop() {
  int option = runOption();
  if (digitalRead(button) == HIGH) {
    if (!mode) { //init button press
      currentNode = mapController.getHead();
      if (!mapController.getPath(currentNode, 1) && !mapController.getPath(currentNode, 2)) {
        currentNode = mapController.getPath(currentNode, 0);
        moveForwardOneCarLength();
      }
    }
    mode = true;
  }

  if (mode) {
    if (option == 0) {
      moveForward();
    } else {
      moveForwardOneCarLength();
      if (mapController.getPath(currentNode, 0) && !mapController.getIsDeadEnd(mapController.getPath(currentNode, 0))) {
        currentNode = mapController.getPath(currentNode, 0);
        moveForwardOneCarLength();
      } else if (mapController.getPath(currentNode, 1) && !mapController.getIsDeadEnd(mapController.getPath(currentNode, 1))) {
        currentNode = mapController.getPath(currentNode, 1);
        turnLeft();
        moveForwardOneCarLength();
      } else if (mapController.getPath(currentNode, 2) && !mapController.getIsDeadEnd(mapController.getPath(currentNode, 2))) {
        currentNode = mapController.getPath(currentNode, 2);
        turnRight();
        moveForwardOneCarLength();
      } else {
        Serial.println("Error mapping || no possible solutions || Maze solved!!");
        for (;;);
      }
    }

  } else {
    if (onDeadEnd) {
      double rightIRSensorValue = getRightIRSensorValue();
      double leftIRSensorValue = getLeftIRSensorValue();
      double frontUSSensorValue = sonar.ping_cm();
      moveBackward();
      if (leftIRSensorValue > distanceFromWalls || rightIRSensorValue > distanceFromWalls) {
        moveBackwardOneCarLength();
        int tempTurn = mapController.getTurn(currentNode);
        currentNode = mapController.getPreviousNode(currentNode);
        switch (tempTurn) {
          case 1:
            turnRight();
            if (!!mapController.getPath(currentNode, 0)) {
              moveForwardOneCarLength();
              onDeadEnd = false;
            } else {
              mapController.makeDeadEnd(currentNode);
              moveBackwardOneCarLength();
            }
            break;
          case 2:
            turnLeft();
            if (!!mapController.getPath(currentNode, 1)) {
              turnLeft();
              moveForwardOneCarLength();
              onDeadEnd = false;
            } else if (!!mapController.getPath(currentNode, 0)) {
              moveForwardOneCarLength();
              onDeadEnd = false;
            } else {
              mapController.makeDeadEnd(currentNode);
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


}

/**
   Check what turning options are available at specific point
   //front has to be right at beginning of intersection
*/
int runOption() {
  double rightIRSensorValue = getRightIRSensorValue(); //using exponential regression stuff that Karan did
  double leftIRSensorValue = getLeftIRSensorValue(); //using exponential regression stuff that Karan did
  double frontUSSensorValue = sonar.ping_cm();
  //  if(frontUSSensorValue > 153) {
  //    return -1;
  //  }
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
  mapController.setLength(currentNode, getBlocksTraveled());
  mapController.addPath(currentNode, 0);
  mapController.addPath(currentNode, 1);
  currentNode = mapController.addPath(currentNode, 2);
}
void sidesThreeWay() {
  mapController.setLength(currentNode, getBlocksTraveled());
  mapController.addPath(currentNode, 1);
  currentNode = mapController.addPath(currentNode, 2);
}
void rightThreeWay() {
  mapController.setLength(currentNode, getBlocksTraveled());
  mapController.addPath(currentNode, 0);
  currentNode = mapController.addPath(currentNode, 2);
}
void leftThreeWay() {
  mapController.setLength(currentNode, getBlocksTraveled());
  mapController.addPath(currentNode, 0);
  currentNode = mapController.addPath(currentNode, 1);
}
void rightTwoWay() {
  mapController.setLength(currentNode, getBlocksTraveled());
  currentNode = mapController.addPath(currentNode, 2);
}
void leftTwoWay() {
  mapController.setLength(currentNode, getBlocksTraveled());
  currentNode = mapController.addPath(currentNode, 1);
}
void deadEnd() {
  mapController.setLength(currentNode, getBlocksTraveled());
  mapController.makeDeadEnd(currentNode);
  onDeadEnd = true;
}




int getBlocksTraveled() {
  int blockEncoderValue = 1; //CHANGE THIS $
  int blocksTraveled = leftWheelEncoder.read() / blockEncoderValue + 1;
  resetEncoders();
  return blocksTraveled;
}






void moveForward() { //$
  analogWrite(rightWheelBck, 0);
  analogWrite(leftWheelBck, 0);
  analogWrite(rightWheelFwd, speed);
  analogWrite(leftWheelFwd, speed);
}
void moveForwardOneCarLength() { //$
  resetEncoders();
  while (leftWheelEncoder.read() < oneCarLength || rightWheelEncoder.read() < oneCarLength) {
    //moveForward();
    if(leftWheelEncoder.read() < oneCarLength) {
      analogWrite(leftWheelFwd, speed);
    } else {
      leftStop();
    }
    if(rightWheelEncoder.read() < oneCarLength) {
      analogWrite(rightWheelFwd, speed);
    } else {
      rightStop();
    }
  }
  stopMoving();
}

void moveBackward() {
  analogWrite(rightWheelFwd, 0);
  analogWrite(leftWheelFwd, 0);
  analogWrite(rightWheelBck, speed);
  analogWrite(leftWheelBck, speed);
}
void moveBackwardOneCarLength() { //$
  resetEncoders();
  while (leftWheelEncoder.read() > -quarterTurn && rightWheelEncoder.read() > -quarterTurn) {
    moveBackward();
  }
  stopMoving();
}

//Stops all motor movement

void stopMoving() {
  leftStop();
  rightStop();
}
void leftStop() {
  analogWrite(leftWheelFwd, 0);
  analogWrite(leftWheelBck, 0);
}
void rightStop() {
  analogWrite(rightWheelFwd, 0);
  analogWrite(rightWheelBck, 0);
}


void turnRight() { //$
  resetEncoders();
  stopMoving();
  while (leftWheelEncoder.read() < quarterTurn && rightWheelEncoder.read() > -quarterTurn) {
    analogWrite(leftWheelFwd, speed);
    analogWrite(rightWheelBck, speed);
  }
  stopMoving();
  resetEncoders();
}

void turnLeft() { //$
  resetEncoders();
  stopMoving();
  while (leftWheelEncoder.read() < -quarterTurn && rightWheelEncoder.read() > quarterTurn) {
    analogWrite(leftWheelBck, speed);
    analogWrite(rightWheelFwd, speed);
  }
  stopMoving();
  resetEncoders();
}

//Resets encoder value to 0 for comparison
void resetEncoders() {
  leftWheelEncoder.write(0);
  rightWheelEncoder.write(0);
}

double getRightIRSensorValue() {
  return (39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3); //using exponential regression stuff that Karan did
}
double getLeftIRSensorValue() {
  return (39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3); //using exponential regression stuff that Karan did
}

