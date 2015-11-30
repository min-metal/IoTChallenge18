#include "State.h"
#include "State_Low.h"


State_Result State_Low::run()
{
  Traffic traffic;
  int amount[100];
  
  
  while(true)
  {
    getTraffic(irRead, irRead, &traffic);

    
  }
}

