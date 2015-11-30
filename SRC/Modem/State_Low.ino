#include "State.h"
#include "State_Low.h"


State_Result State_Low::run()
{
  Serial.print("hello World");

  Traffic traffic;
  
  while(true)
  {
    getTraffic(irRead, irRead, &traffic);

    
  }
}

