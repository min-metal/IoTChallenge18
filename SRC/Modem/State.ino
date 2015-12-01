#include "State.h"
State::State(int * thresholdPoints, int * freq, int area, LoRaModem * modem)
{
  this->currentState = HI_STATE;
  this->areaID = area;
  this->modem = modem;

  for(int i = 0; i < NUM_THRESHOLD; ++i)
  {
    this->thresholdPoints[i] = thresholdPoints[i];
  }

  for(int i = 0; i < NO_STATE; ++i)
  {
    this->msgFrequencies[i] = freq[i];
  }
};

void State::setArea(int area)
{
  this->areaID = area;
};


State_Result State::run()
{
  Traffic traffic;
  unsigned short MAX = this->msgFrequencies[this->currentState]; //5; 
  long trafficHistory[MAX], netTrafficForPeriod = 0;
  unsigned long timeHistory[MAX], timeForPeriod = 0;

  Serial.print("Max: ");
  Serial.println(MAX);
  Serial.print("lower: ");
  Serial.println(this->thresholdPoints[this->currentState]);
  Serial.print("upper: ");
  Serial.println(this->thresholdPoints[this->currentState + 1]);
  Serial.print("freq: ");
  Serial.println(this->msgFrequencies[this->currentState]);




  unsigned short count = 0;

  memset(trafficHistory, 0, sizeof(long) * MAX);
  memset(timeHistory, 0, sizeof(unsigned long) * MAX);
  
  while(true)
  {
    getTraffic(&traffic, 5); // TODO change
    trafficHistory[count] = traffic.in - traffic.out;
    timeHistory[count] = traffic.deltaTime;
    ++count;

    netTrafficForPeriod = timeForPeriod = 0;
    for(int i = 0; i < MAX; ++i)
    {
      netTrafficForPeriod += trafficHistory[i];
      timeForPeriod += timeHistory[i]; 
    }
    
    double averageRate = (double) netTrafficForPeriod/timeForPeriod; // this is the average rate for the last MAX (20) readings

    if(averageRate < this->thresholdPoints[this->currentState])
    {
      // return decrease_state
    }
    if(averageRate >= this->thresholdPoints[this->currentState + 1])
    {
      // return increase_state
    }
    
    Serial.print("In: ");
    Serial.println(traffic.in);
    Serial.print("Out: ");
    Serial.println(traffic.out);
    Serial.print("DetlaTime: ");
    Serial.println(traffic.deltaTime);
    Serial.print("Average Rate(ppl/s)");
    Serial.println(averageRate * 1000);

    if(count % MAX == 0)
    {
      // send some message to server
      this->modem->Msg("I did a period");
      count = 0;
    }

    
  }
}

