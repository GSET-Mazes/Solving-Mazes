//Imports necessary libraries for sensors
//Imports necessary libraries for sensors
#include <Encoder.h>
#include <NewPing.h>

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
  delay(10000);
}

void loop() {
//  for(;;);
//  Serial.println(runOption());
//  moveForwardOneCarLength();
moveForward();

  
//  int option = runOption();
//  if(option == 0 || option == 2) {
//    moveForwardOneCarLength();
//  } else if(option == 1) {
//    turnRight();
//    moveForwardOneCarLength();
//  } else if(option == 3) {
//    turnLeft();
//    moveForwardOneCarLength();
//  } else if(option == 4) {
//    turnLeft();
//    turnLeft();
//  }
}

int runOption() {
  double rightIRSensorValue = getRightIRSensorValue();
  double leftIRSensorValue = getLeftIRSensorValue();
  double frontUSSensorValue = sonar.ping_cm();
  if(rightIRSensorValue > distanceFromWalls) { //right is an option
    return 1;
  } else if(frontUSSensorValue > distanceFromWalls) { //right not an option; straight is an option
    return 2;
  } else if(leftIRSensorValue > distanceFromWalls) { //only left is an option
    return 3;
  } else if(frontUSSensorValue < distanceFromWalls) { //dead end
    return 4;
  }
  return 0;
}



void moveForward() { //$
  analogWrite(rightWheelBck, 0);
  analogWrite(leftWheelBck, 0);
  analogWrite(rightWheelFwd, 100);
  analogWrite(leftWheelFwd, 100);
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


void turnRight() { 
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
