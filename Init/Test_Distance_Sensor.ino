
#include <SharpDistSensor.h>


const byte distSensor = A0;

const byte mediumFilterWindowSize = 1;
SharpDistSensor sensor(sensorPin, mediumFilterWindowSize);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Get distance from sensor
  unsigned int distance = sensor.getDist();

  // Print distance to Serial
  Serial.println(distance);

  // Wait some time
  delay(50);
}
