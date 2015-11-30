#ifndef STATE
#define STATE

#include "Modem.h"
#include "IR_Sensor.h"

class State
{
  public:
    State(int lower, int upper, int area, LoRaModem * modem);
    int lowerThreshold;
    int upperThreshold;
    int areaID;

  private:
    LoRaModem * _modem;
    int currentTrafficRating;
};



typedef struct 
{
  boolean nextState;
} State_Result;


#endif

