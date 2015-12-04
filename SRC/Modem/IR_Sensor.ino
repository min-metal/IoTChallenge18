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
  digitalWrite(enPin,LOW);
  pinMode(enPin2, OUTPUT);
  digitalWrite(enPin2,LOW);
  
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

/*
 * @param readTime: how long do you want the function to get data for. (in seconds)
 */
int getTraffic(Traffic * traffic, unsigned short readTime)
{
  int DELAY = 100;

  byte ir1, ir2;
  unsigned int in = 0, out = 0;
  unsigned long timeStart, timeEnd;
  unsigned int x = (unsigned int) ((unsigned int) readTime * 1000 / DELAY);

  //* Can overflow (in 50 days)*/
  timeStart = millis();

  
  for(int i = 0; i < x; ++i) //if readTime == 5, for approx. 5 seconds (50 cycles w/ 100mS delay + reading time + execution time), read sensors
  {
    digitalWrite(enPin,HIGH);
//    digitalWrite(enPin2,HIGH);
    ir1 = irRead(irSensorPin, irLedPin);
    ir2 = irRead(irSensorPin2, irLedPin2); 
    digitalWrite(enPin,LOW);
//    digitalWrite(enPin2,LOW);

    timeEnd = millis();
    if(timeEnd - timeStart > (unsigned long) readTime * 1000)
    {
        traffic->in = in;
        traffic->out = out;
        traffic->deltaTime = timeEnd - timeStart;
        return in - out;
    }

    if(!ir1 && !ir2)
    {
      delay(DELAY);
      continue;
    }
    
    if(ir1 == CLOSED)
    {
      boolean success = false;
      // for approx 0.5 seconds (5 cycles of 100 mS) , read value of 2nd sensor
      for(int i = 0; i < 5; ++i)
      {
        digitalWrite(enPin,HIGH);
        ir2 = irRead(irSensorPin2 ,irLedPin2);
        digitalWrite(enPin,LOW);

        if(ir2 == CLOSED)
        {
          out++;
          Serial.println("Someone Exited");
          delay(10);
          success = true;
          break;
        }

        if(success)
          break;
               
        delay(DELAY);
      } 
    }
    else if(ir2 == CLOSED)
    {
      boolean success = false;
      // for approx 0.5 seconds, read value of 1st sensor
      for(int i = 0; i < 5; ++i)
      {
        digitalWrite(enPin,HIGH);
        ir1 = irRead(irSensorPin, irLedPin);
        digitalWrite(enPin,LOW);

        if(ir1 == CLOSED)
        {
          in++;
          Serial.println("Someone Entered");
          delay(10);
          success = true;
          break;
        }

        if(success)
          break;           
        delay(DELAY);
      } 
    }
    
    delay(DELAY);
  }

  digitalWrite(enPin,LOW);
  digitalWrite(enPin2,LOW);

  timeEnd = millis();

  traffic->in = in;
  traffic->out = out;
  traffic->deltaTime = timeEnd - timeStart;

  return in - out; 
}

