#include "State.h"
State::State(double * thresholdPoints, unsigned short * freq, int area, LoRaModem * modem)
{
  this->currentState = AVG;
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

  unsigned short pingHowOften = this->msgFrequencies[this->currentState];
  unsigned short READ_DURATION = 30; // hard_coded value of how long getTraffic will run (in seconds)
  unsigned short MAX =  pingHowOften * 60 / READ_DURATION + 1;//20; //max value for history
  int trafficHistory[MAX], netTrafficForPeriod = 0;
  unsigned int timeHistory[MAX];
  unsigned long timeForPeriod = 0;
  memset(trafficHistory, 0, sizeof(short) * MAX);
  memset(timeHistory, 0, sizeof(unsigned int) * MAX);


  Serial.print("Max: ");
  Serial.println(MAX);
  Serial.print("lower: ");
  Serial.println(this->thresholdPoints[this->currentState]);
  Serial.print("upper: ");
  Serial.println(this->thresholdPoints[this->currentState + 1]);
  Serial.print("freq: ");
  Serial.println(this->msgFrequencies[this->currentState]);
  delay(100);

  unsigned long timeStart = millis();
  unsigned long timeEnd = timeStart + 60000*pingHowOften;

  unsigned short count = 0;
  while(true)
  {
    getTraffic(&traffic, READ_DURATION);
    trafficHistory[count] = traffic.in - traffic.out;
    timeHistory[count] = traffic.deltaTime;
    ++count;

    netTrafficForPeriod = 0;
    timeForPeriod = 0;
    for(int i = 0; i < MAX; ++i)
    {
      netTrafficForPeriod += trafficHistory[i];
      timeForPeriod = timeForPeriod + timeHistory[i]; 
    }

    for(int i = 0; i<MAX; ++i)
    {
      Serial.print("trafficHistory: ");
      Serial.println(trafficHistory[i]);
      Serial.print("timeHistory: ");
      Serial.println(timeHistory[i]); 
    }

    
    Serial.print("netTrafficForPeriod: ");
    Serial.println(netTrafficForPeriod);
    Serial.print("timeForPeriod: ");
    Serial.println(timeForPeriod);
    
    double averageRate = (double) netTrafficForPeriod/ (double)timeForPeriod;
    
    Serial.print("In: ");
    Serial.println(traffic.in);
    Serial.print("Out: ");
    Serial.println(traffic.out);
    Serial.print("DetlaTime: ");
    Serial.println(traffic.deltaTime);
    Serial.print("Average Rate(ppl/s)");
    Serial.println(averageRate * 1000);
    delay(1000);

    if(count % MAX == 0)
    {
      count = 0;
    }
    
    if(millis() >= timeEnd)
    {
       
      if(averageRate*1000 < this->thresholdPoints[this->currentState]
          && this->currentState != HI_OUT)
      {
        DEBUG_PRINT("moving down state");
        return PREV_STATE;
      }
      else if(averageRate*1000 >= this->thresholdPoints[this->currentState + 1]
          && this->currentState != HI_IN)
      {
        DEBUG_PRINT("moving up state");
        return NEXT_STATE;
      }
      else {
        this->modem->Msg("I did a period");
        return SAME_STATE;
      }
    }


    
  }
}

