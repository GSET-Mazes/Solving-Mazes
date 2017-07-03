//Imports necessary libraries for sensors
#include <Map.h>

Map mapStart;


//Initiates Session
void setup() {
  Map rightOne = mapStart.addPath(2);
  rightOne.setLength(59);
  Serial.println(rightOne.getLength());
  mapStart.getPath(2);
}

void loop() {
  
  
}

