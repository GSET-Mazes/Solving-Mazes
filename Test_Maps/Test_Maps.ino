//Imports necessary libraries for sensors
#include <Map.h>
#include <NewPing.h>
#include <Encoder.h>
//Map mapStart;
//Node *curr;

const byte button = 8;
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
const byte led = 13;

Encoder leftWheelEncoder(leftEncoder1, leftEncoder2);
Encoder rightWheelEncoder(rightEncoder1, rightEncoder2);
NewPing sonar(ultraSonic, ultraSonic, 3000);

bool mode = true;
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
  delay(20000);
}


void loop() {
//  analogWrite(rightWheelFwd, 30);
  moveForwardOneCarLength();
  if(getRightIRSensorValue() > 7) {
    for(;;);
  }
//  if(mode) {
//    moveForwardOneCarLength();
//    moveForwardOneCarLength();
////    turnRight();
////    moveForwardOneCarLength();
////    turnRight();
////    moveForwardOneCarLength();
////    turnLeft();
////    moveForwardOneCarLength();
////    moveForwardOneCarLength();
////    turnLeft();
////    moveForwardOneCarLength();
////    moveForwardOneCarLength();
////    moveForwardOneCarLength();
////    turnLeft();
////    moveForwardOneCarLength();
////    moveForwardOneCarLength();
////    turnRight();
////    moveForwardOneCarLength();
////    moveForwardOneCarLength();
////    turnRight();
//    
////    moveForwardOneCarLength();
//    mode = false;
//  }
//  moveForwardOneCarLength();
}

double getRightIRSensorValue() {
  return (39.4527 * (pow(0.0614007, (analogRead(rightIRSensor) / 200.0))) + 2.3); //using exponential regression stuff that Karan did
}
double getLeftIRSensorValue() {
  return (39.4527 * (pow(0.0614007, (analogRead(leftIRSensor) / 200.0))) + 2.3); //using exponential regression stuff that Karan did
}


void moveForward() {
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
  while (leftWheelEncoder.read() > -quarterTurn && rightWheelEncoder.read() < quarterTurn) {
    analogWrite(leftWheelBck, speed);
    analogWrite(rightWheelFwd, speed);
  }
  stopMoving();
  resetEncoders();
}
void resetEncoders() {
  leftWheelEncoder.write(0);
  rightWheelEncoder.write(0);
}
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

