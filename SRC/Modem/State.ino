#include "State.h"
#define IN      100
#define OUT     101
#define NET     102
#define TIME    103
#define STATE   104
#define AREA    105

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
  unsigned short READ_DURATION = 20; // hard_coded value of how long getTraffic will run (in seconds)
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
    double averageRate = (double) netTrafficForPeriod/ (double)timeForPeriod;
    
    if(millis() >= timeEnd)
    {

      if(averageRate*1000 < this->thresholdPoints[this->currentState]
          && this->currentState != HI_OUT)
      {
        DEBUG_PRINT("moving down state");
        char prompt[20];

        this->currentState--;

        State_Result result;
        result.in = allIn;
        result.out = allOut;
        result.Time = timeForPeriod;
//        result.next = this->currentState;
        result.area = this->areaID;

        return result;
      }
      else if(averageRate*1000 >= this->thresholdPoints[this->currentState + 1]
          && this->currentState != HI_IN)
      {
        DEBUG_PRINT("moving up state");
        char prompt[20];

        this->currentState++;
        
        State_Result result;
        result.in = allIn;
        result.out = allOut;
        result.Time = timeForPeriod;
//        result.next = this->currentState;
        result.area = this->areaID;

        return result;
      }
      else {
        char prompt[20];
//        sprintf(prompt, "101, I=%diO=%doN=%dnT=%lutS=%dsA=%da", 
//        allIn, allOut, netTrafficForPeriod, timeForPeriod, this->currentState, this->areaID);
        
//        sprintf(prompt, "%d,%d", IN, allIn);
//        this->modem->Msg(prompt);
//        sprintf(prompt, "%d,%d", OUT, allOut);
//        this->modem->Msg(prompt);
//        sprintf(prompt, "%d,%d", NET, netTrafficForPeriod);
//        this->modem->Msg(prompt);
//        sprintf(prompt, "%d,%lu", TIME, timeForPeriod / 1000);
//        this->modem->Msg(prompt);
//        sprintf(prompt, "%d,%d", STATE, this->currentState);
//        this->modem->Msg(prompt);
//        sprintf(prompt, "%d,%d", AREA, this->areaID);
//        this->modem->Msg(prompt);
              
        State_Result result;
        result.in = allIn;
        result.out = allOut;
        result.Time = timeForPeriod;
//        result.next = this->currentState;
        result.area = this->areaID;

        return result;
      }
    }
    
  }
}

