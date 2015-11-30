#ifndef STATE
#define STATE

#include "Modem.h"
#include "IR_Sensor.h"

#define NO_STATE 3
#define NUM_THRESHOLD NO_STATE + 1

// no. of enums must be 1 less than NUM_THRESHOLD
typedef enum
{
  LOW_STATE = 0, MED_STATE, HI_STATE
} CURRENT_STATE;

typedef struct 
{
  boolean nextState;
} State_Result;

class State
{
  public:
    State(int * thresholdPoints, int *, int area, LoRaModem * modem);
    LoRaModem * modem;
    byte currentState;
    int thresholdPoints[NUM_THRESHOLD];
    int msgFrequencies[NO_STATE]; //How often it sends stuff back to server, dependent on state
    int areaID;
    void setArea(int area);   
    State_Result run(void);

  private:
};

#endif

