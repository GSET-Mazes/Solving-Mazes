//Imports necessary libraries for sensors
#include <Map.h>

Map mapStart;
Node *curr;

//Initiates Session
void setup() {
  Serial.begin(9600);
  curr = mapStart.addPath(mapStart.getHead(), 0);
}

void loop() {
  mapStart.setLength(curr, 13);
  Node *start = mapStart.getPreviousNode(curr);
//  Map previousNode = rightOne.getPreviousNode();
//  int daLength = rightOne.getLength();
//  int newX = previousNode.getX() + daLength;
//  int i = rightOne.getX();
  Serial.println(!!mapStart.getPath(start, 1));
  delay(200);
}

