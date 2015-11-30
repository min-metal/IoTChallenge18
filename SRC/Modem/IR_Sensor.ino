// Sourced from http://playground.arduino.cc/Main/PanasonicIrSensor


#include "IR_Sensor.h"


int irRead(int readPin, int triggerPin); //function prototype

void IR_setup()
{
  pinMode(irSensorPin, INPUT);
  pinMode(irLedPin, OUTPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irLedPin2, OUTPUT);
  
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,HIGH);
  pinMode(enPin2, OUTPUT);
  digitalWrite(enPin2,HIGH);
  
}

/******************************************************************************
 * This function can be used with a panasonic pna4602m ir sensor
 * it returns a zero if something is detected by the sensor, and a 1 otherwise
 * The function bit bangs a 38.5khZ waveform to an IR led connected to the
 * triggerPin for 1 millisecond, and then reads the IR sensor pin to see if
 * the reflected IR has been detected
 ******************************************************************************/
int irRead(int readPin, int triggerPin)
{
  int halfPeriod = 13; //one period at 38.5khZ is aproximately 26 microseconds
  int cycles = 38; //26 microseconds * 38 is more or less 1 millisecond
  int i;
  for (i=0; i <=cycles; i++)
  {
    digitalWrite(triggerPin, HIGH); 
    delayMicroseconds(halfPeriod);
    digitalWrite(triggerPin, LOW); 
    delayMicroseconds(halfPeriod - 1);     // - 1 to make up for digitaWrite overhead    
  }
  return digitalRead(readPin);
}

int getTraffic(int (*readOne) (int, int), int (*readTwo) (int, int), Traffic * traffic)
{
  int ir1, ir2;
  int in = 0, out = 0;

  ir1 = readOne(irSensorPin, irLedPin);
  ir2 = readTwo(irSensorPin2, irLedPin2); 

  Serial.print("ir1: ");
  Serial.println(ir1);
  Serial.print("ir2: ");
  Serial.println(ir2);
  
  delay(1000);
  
//
//  for(int i = 0; i < 100; ++i) //for approx. 10 seconds (100 cycles w/ 100mS delay), read sensors
//  {
//    ir1 = readOne(irSensorPin, irLedPin);
//    ir2 = readTwo(11, 12); //hardcoded for now
//
//    if(ir1 == CLOSED)
//    {
//      // for approx 2 seconds (100 cycles of 20mS) , read value of 2nd sensor
//      for(int i = 0; i < 100; ++i)
//      {
//        ir2 = readTwo(11 ,12);
//        delay(20);
//
//        if(ir2 == CLOSED)
//        {
//          out++;
//          Serial.println("Someone Exited");
//          break;
//        }
//      } 
//    }
//
//    if(ir2 == CLOSED)
//    {
//      // for approx 2 seconds, read value of 2nd sensor
//      for(int i = 0; i < 100; ++i)
//      {
//        ir2 = readTwo(11, 12);
//        delay(20);
//
//        if(ir1 == CLOSED)
//        {
//          in++;
//          Serial.println("Someone Entered");
//          break;
//        }
//      } 
//    }
//    
//    delay(100);
//  }

  traffic->in = in;
  traffic->out = out;

  return in - out; 
}

