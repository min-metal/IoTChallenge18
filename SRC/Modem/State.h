#ifndef STATE
#define STATE

#include "Modem.h"
#include "IR_Sensor.h"

#define NO_STATE 5
#define NUM_THRESHOLD NO_STATE + 1

// no. of enums must be 1 less than NUM_THRESHOLD
typedef enum
{
  HI_OUT = 0, MED_OUT, AVG, MED_IM, HI_IN
} CURRENT_STATE;

typedef enum 
{
  PREV_STATE, SAME_STATE, NEXT_STATE
} State_Result;

class State
{
  public:
    State(double * thresholdPoints, unsigned short *, int area, LoRaModem * modem);
    LoRaModem * modem;
    byte currentState;
    double thresholdPoints[NUM_THRESHOLD];
    unsigned short msgFrequencies[NO_STATE]; //How often it sends stuff back to server, dependent on state
    int areaID;
    void setArea(int area);   
    State_Result run(void);

  private:
};

#endif

