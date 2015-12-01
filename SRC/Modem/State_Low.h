#ifndef STATE_LOW
#define STATE_LOW

class State_Low: public State
{
  public:
    State_Low(int lower, int upper, int area, LoRaModem * modem)
    : State(lower, upper, area, modem)
    {
      
    }

    State_Result run(void);
  
};

#endif

