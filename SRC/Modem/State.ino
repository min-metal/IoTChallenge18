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
  int netTrafficForPeriod = 0, allIn = 0, allOut = 0;
  unsigned long timeForPeriod = 0;

//  Serial.print("lower: ");
//  Serial.println(this->thresholdPoints[this->currentState]);
//  Serial.print("upper: ");
//  Serial.println(this->thresholdPoints[this->currentState + 1]);
//  Serial.print("freq: ");
//  Serial.println(this->msgFrequencies[this->currentState]);
//  delay(100);

  unsigned long timeStart = millis();
  unsigned long timeEnd = timeStart + 60000*pingHowOften;

  unsigned short count = 0;
  while(true)
  {
    getTraffic(&traffic, READ_DURATION);
    allIn += traffic.in;
    allOut += traffic.out;

    netTrafficForPeriod = allIn - allOut;
    timeForPeriod += traffic.deltaTime;

    Serial.print("allIn: ");
    Serial.println(allIn);
    Serial.print("allOut: ");
    Serial.println(allOut);
    Serial.print("netTrafficForPeriod: ");
    Serial.println(netTrafficForPeriod);
    Serial.print("timeForPeriod: ");
    Serial.println(timeForPeriod);
    
    double averageRate = (double) netTrafficForPeriod/ (double)timeForPeriod;
    Serial.print("Average Rate(ppl/s)");
    Serial.println(averageRate * 1000);
    delay(1000);
    
    if(millis() >= timeEnd)
    {
       
      if(averageRate*1000 < this->thresholdPoints[this->currentState]
          && this->currentState != HI_OUT)
      {
        DEBUG_PRINT("moving down state");
        char prompt[40];

        this->currentState--;
        sprintf(prompt, "222, I=%d:O=%d:N=%d:TME=%lu:S=%d:A=%d", 
        allIn, allOut, netTrafficForPeriod, timeForPeriod, this->currentState, this->areaID);
        this->modem->Msg(prompt);
        return PREV_STATE;
      }
      else if(averageRate*1000 >= this->thresholdPoints[this->currentState + 1]
          && this->currentState != HI_IN)
      {
        DEBUG_PRINT("moving up state");
        char prompt[80];

        this->currentState++;
        sprintf(prompt, "222, I=%d:O=%d:N=%d:TME=%lu:S=%d:A=%d", 
        allIn, allOut, netTrafficForPeriod, timeForPeriod, this->currentState, this->areaID);
        this->modem->Msg(prompt);
        return NEXT_STATE;
      }
      else {
        char prompt[40];
        sprintf(prompt, "222, I=%d:O=%d:N=%d:TME=%lu:S=%d:A=%d", 
        allIn, allOut, netTrafficForPeriod, timeForPeriod, this->currentState, this->areaID);
        this->modem->Msg(prompt);
        
        return SAME_STATE;
      }
    }


    
  }
}

