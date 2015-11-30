#include "Traffic.h"
#ifndef IR_SENSOR
#define IR_SENSOR

#define enPin 9
#define irLedPin 11          // IR Led on this pin
#define irSensorPin 10       // IR sensor on this pin

#define enPin2 12
#define irLedPin2 6
#define irSensorPin2 5

#define OPEN 1
#define CLOSED 0 //if the ir sensor is covered

int irRead(int readPin, int triggerPin);

int getTraffic(Traffic * traffic, int);

#endif

